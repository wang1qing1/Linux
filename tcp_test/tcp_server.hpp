#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include "err.hpp"
#include "mythread.hpp"

using namespace std;

const int static backlog = 32;
//  host <-> network <arpa/inet.h>
//  htonl() 将无符号整数,从主机字节顺序转换为网络字节顺序。
//  htons() 将无符号短整数,从主机字节顺序转换为网络字节顺序。
//  ntohl() 将无符号整数,从网络字节顺序转换为主机字节顺序。
//  ntohs() 将无符号短整数,从网络字节顺序转换为主机字节顺序。

class tcp_server
{
public:
    tcp_server(uint16_t port, function<string(string)> func)
        : _port(port), _func(func)
    {
    }

    void Init()
    {
        // 1.创建套接字
        // AF_INET：使用IPv4通信。
        // SOCK_STREAM:面向字节流,可靠的，有序，双向的，tcp通信。
        _listensock = socket(AF_INET, SOCK_STREAM, 0);
        if (_listensock == -1)
        {
            cerr << "socket err:" << strerror(errno) << endl;
            exit(SOCKER_ERR);
        }
        else
        {
            cerr << "socket successful" << endl;
        }

        // 2.绑定
        struct sockaddr_in host;
        memset(&host, 0, sizeof(host));
        host.sin_family = AF_INET; // #define AF_INET 2
        host.sin_port = htons(_port);
        host.sin_addr.s_addr = htonl(INADDR_ANY); // #define INADDR_ANY 0
        if (bind(_listensock, (struct sockaddr *)&host, sizeof(host)) == -1)
        {
            cerr << "bind err:" << strerror(errno) << endl;
            exit(BIND_ERR);
        }
        else
        {
            cerr << "bind successful" << endl;
        }

        // 3.监听
        if (listen(_listensock, backlog) == -1)
        {
            cerr << "listen err:" << strerror(errno) << endl;
            exit(LISTEN_ERR);
        }
        else
        {
            cerr << "listen successful" << endl;
        }
    }

    struct Args
    {
        Args(int sock, uint16_t port, string ip, tcp_server *server)
            : _sock(sock), _port(port), _ip(ip), _server(server)
        {
        }
        int _sock;
        uint16_t _port;
        string _ip;
        tcp_server *_server;
    };

    void Start()
    {
        // v2.1子进程回收
        signal(SIGCHLD, SIG_IGN);

        while (1)
        {
            // 4.接受请求
            // 表明接受的客户端主机
            struct sockaddr_in client_host;
            socklen_t client_len = sizeof(client_host);
            int sock = accept(_listensock, (struct sockaddr *)&client_host, &client_len);
            if (sock == -1)
            {
                cerr << "accpet err:" << strerror(errno) << endl;
                exit(ACCEPT_ERR);
            }
            else
            {
                cerr << "socket successful,get a new client"
                     << "fd:" << sock << endl;
            }

            // 提取客户端信息
            uint16_t client_port = ntohs(client_host.sin_port);
            // inet_ntoa: 将网络主机地址(以网络字节序)转换为IPv4点分十进制的字符串.
            string client_ip = inet_ntoa(client_host.sin_addr);
            // <-------------------------------------------------------------------------->
            // 处理接受的客户端  v1——只有一个客户端
            // service(sock, client_port, client_ip);

            // <-------------------------------------------------------------------------->
            // 处理接受的客户端 V2.1——多进程版本
            // int pid = fork();
            // if (pid == 0) // 子进程
            // {
            //     service(sock, client_port, client_ip);
            //     exit(0);
            // }
            // close(sock);

            // <-------------------------------------------------------------------------->
            // 处理接受的客户端 V2.2——多进程版本
            // int pid = fork();
            // if (pid == 0) // 子进程
            // {
            //     //  创建孙子进程，再让子进程退出，让孙子进程编程孤儿进程，
            //     //  孤儿进程又操作系统管理，无需手动等待孙子进程
            //     if (fork() > 0)
            //         exit(0);
            //     service(sock, client_port, client_ip);
            //     exit(0);
            // }
            // close(sock);

            // <-------------------------------------------------------------------------->
            // 处理接受的客户端 V3——多线程版本

            // Thread t1(1, bind(&tcp_server::threadRun, this, placeholders::_1), new Args(sock, client_port, client_ip));
            // t1.run();
            pthread_t t;
            pthread_create(&t, nullptr, threadRun, new Args(sock, client_port, client_ip, this));
        }
    }

    static void *threadRun(void *args)
    {
        pthread_detach(pthread_self());
        Args *ts = (Args *)(args);
        ts->_server->service(ts->_sock, ts->_port, ts->_ip);
        return nullptr;
    }

    void service(int sock, uint16_t client_port, string client_ip)
    {
        string name = to_string(client_port) + "-" + client_ip;
        char buff[1024];
        while (1)
        {
            int n = read(sock, buff, sizeof(buff) - 1);
            if (n > 0) // 读取成功
            {
                buff[n] = 0;
                // 读取成功之后，处理也业务
                string recvmess = _func(name + ">>" + buff);
                // 将信息返回
                write(sock, recvmess.c_str(), recvmess.size());
            }
            else if (n == 0)
            {
                // 对方将连接关闭了
                close(sock);
                std::cout << name << " quit, me too" << std::endl;
                break;
            }
            else
            {
                close(sock);
                cout << "read err" << strerror(errno) << endl;
                break;
            }
        }
    }

private:
    uint16_t _port;                 // 端口
    int _listensock;                // 监听套接字
    function<string(string)> _func; // 服务端处理的业务
};