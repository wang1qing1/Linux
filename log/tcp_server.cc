
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
#include "log.hpp"

using namespace std;
const int static backlog = 32;

enum err
{
    SOCKER_ERR = 1,
    BIND_ERR,
    LISTEN_ERR,
    ACCEPT_ERR,
    CONNECT_ERR
};
class tcp_server;
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
            Logmessage(Fatal, "socket error,error code:%d,%s", errno, strerror(errno));
            exit(SOCKER_ERR);
        }
        else
        {
            Logmessage(Debug, "socket successful");
        }

        // 2.绑定
        struct sockaddr_in host;
        memset(&host, 0, sizeof(host));
        host.sin_family = AF_INET; // #define AF_INET 2
        host.sin_port = htons(_port);
        host.sin_addr.s_addr = htonl(INADDR_ANY); // #define INADDR_ANY 0
        if (bind(_listensock, (struct sockaddr *)&host, sizeof(host)) == -1)
        {
            Logmessage(Fatal, "bind error,error code:%d,%s", errno, strerror(errno));
            exit(BIND_ERR);
        }
        else
        {
            Logmessage(Debug, "bind successful");
        }

        // 3.监听
        if (listen(_listensock, backlog) == -1)
        {
            Logmessage(Fatal, "listrn error,error code:%d,%s", errno, strerror(errno));
            exit(LISTEN_ERR);
        }
        else
        {
            Logmessage(Debug, "listen successful");
        }
    }

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
                Logmessage(Error, "accept error,error code:%d,%s", errno, strerror(errno));
                exit(ACCEPT_ERR);
            }
            else
            {
                Logmessage(Debug, "listen successful,sock fd:%d", sock);
            }

            // 提取客户端信息
            uint16_t client_port = ntohs(client_host.sin_port);
            // inet_ntoa: 将网络主机地址(以网络字节序)转换为IPv4点分十进制的字符串.
            string client_ip = inet_ntoa(client_host.sin_addr);

            // 多线程处理接受的客户端
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
                cout << "client:" << buff << endl;
                // 读取成功之后，处理也业务
                string recvmess = _func(name + ":" + buff);
                // 将信息返回
                write(sock, recvmess.c_str(), recvmess.size());
            }
            else if (n == 0)
            {
                // 对方将连接关闭了
                close(sock);
                Logmessage(Debug, "client quit,me too");
                break;
            }
            else
            {
                close(sock);
                Logmessage(Error, "read error,error code:%d,%s", errno, strerror(errno));
                break;
            }
        }
    }

private:
    uint16_t _port;                 // 端口
    int _listensock;                // 监听套接字
    function<string(string)> _func; // 服务端处理的业务
};

string func(string message)
{
    return message + "(-_-)";
}

int main(int argc, char *argv[])
{
    int port = atoi(argv[1]);
    tcp_server server(port, func);
    server.Init();
    server.Start();

    return 0;
}