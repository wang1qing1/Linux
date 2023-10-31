#pragma once
#include <iostream>
#include <unordered_map>
#include <cstdio>
#include <cstring>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "RingQueue.hpp"
#include "mutex.hpp"
#include "mythread.hpp"

using namespace std;
const static int port = 8080;

class Udp_server
{
    enum
    {
        scket_err = 1,
        bind_err
    };

public:
    Udp_server(int po = port)
        : _port(po)
    {
        pthread_mutex_init(&_mutex, nullptr);

        c = new Thread(1, bind(&Udp_server::Recv, this, placeholders::_1), nullptr);
        p = new Thread(2, bind(&Udp_server::broadcast, this, placeholders::_1), nullptr);
    }

    void Init()
    {
        // 一.创建套接字，返回文件描述符，
        // 1.AF_INET:表示支持IPv4网络通信
        // 2.SOCK_DGRAM:支持无连接的不可靠的面向数据报的传输方式，即udp传输
        // 3.0:只支持一个协议来给定套接字
        _fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_fd == -1)
        {
            cout << "create socket error:" << strerror(errno) << endl;
            exit(scket_err);
        }
        else
        {
            cout << "create socket successful" << endl;
        }

        struct sockaddr_in local;
        // 清空
        bzero(&local, sizeof(local));
        // 写入端口号,htons：主机序列转网络序列
        local.sin_port = htons(_port);
        // 写入AF_INET,表示网络通信。
        local.sin_family = AF_INET;
        // 写入IP地址,inet_addr:
        // 1.将点分十进制ip，转化为整形，
        // 2.并且将主机序列转换为网络序列
        // 3.最为一款云服务器，或者服务器，一般不止一个网卡，或者IP,在设置IP时不能直接将ip设置死
        local.sin_addr.s_addr = INADDR_ANY;
        // 二.绑定
        if (bind(_fd, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            cerr << "bind error:" << strerror(errno) << endl;
            exit(bind_err);
        }
        else
        {
            cout << "bind successful" << endl;
        }
    }
    void Start()
    {
        c->run();
        p->run();

        c->join();
        p->join();
    }

    void *Recv(void *argc)
    {
        _Recv();
        return nullptr;
    }

    void *broadcast(void *argc)
    {
        _broadcast();
        return nullptr;
    }

    void _Recv()
    {
        // 1.接受信息
        char buff[1024];
        struct sockaddr_in client;
        socklen_t client_len = sizeof(client);
        while (1)
        {
            int read_size = recvfrom(_fd, buff, sizeof(buff), 0, (struct sockaddr *)&client, &client_len);
            if (read_size > 0)
            {
                buff[read_size] = 0;
            }

            // 提取客户端信息
            string clientip = inet_ntoa(client.sin_addr); // ip
            int clientport = ntohs(client.sin_port);
            // 采集用户信息
            string name = clientip;
            name += "-";
            name += clientport;
            onlineuser[name] = client;
            // 用户消息保存
            string mess = name + buff;
            _rq.push(mess);

            // // 发送信息
            // char send[1024];
            // sprintf(send, "%s (-_-)", buff);
            // sendto(_fd, send, sizeof(send), 0, (const struct sockaddr *)&client, client_len);
        }
    }

    // 将用户信息推送给每一个在线用户
    void _broadcast()
    {
        while (1)
        {
            // 获取消息
            string mes = _rq.pop();
            // 广播给在线用户
            for (auto &e : onlineuser)
            {
                sendto(_fd, mes.c_str(), mes.length(), 0, (struct sockaddr *)&e.second, sizeof(e.second));
            }
        }
    }

    ~Udp_server()
    {
        pthread_mutex_destroy(&_mutex);
        delete c;
        delete p;
    }

private:
    int _port;
    int _fd;
    RingQueue<string> _rq;                                // 循环队列
    unordered_map<string, struct sockaddr_in> onlineuser; // 记录在线用户
    pthread_mutex_t _mutex;                               // 锁
    Thread *c;
    Thread *p;
};
