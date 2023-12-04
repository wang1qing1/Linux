#pragma once
#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "Log.hpp"
#define TCP SOCK_STREAM
#define UDP SOCK_DGRAM
const static int backlog = 1;

enum
{
    SOCK_ERR = 10,
    BING_ERR,
    LISTEN_ERR,
    CONNECT_ERR
};

class Udp
{
public:
    Udp(int SOCK)
    {
        _listensock = socket(AF_INET, SOCK, 0);
        if (_listensock == -1)
        {
            Logmessage(Fatal, "socket err ,error code %d,%s", errno, strerror(errno));
            exit(SOCK_ERR);
        }
    }
    Udp(uint16_t port, int SOCK)
        : _port(port)
    {
        _listensock = socket(AF_INET, SOCK, 0);
        if (_listensock == -1)
        {
            Logmessage(Fatal, "socket err ,error code %d,%s", errno, strerror(errno));
            exit(10);
        }
    }

    void Bind()
    {

        // 设置无需等待TIME_WAIT状态
        int opt = 1;
        setsockopt(_listensock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        struct sockaddr_in host;
        host.sin_family = AF_INET;
        host.sin_port = htons(_port);
        host.sin_addr.s_addr = INADDR_ANY; // #define INADDR_ANY 0x00000000
        socklen_t hostlen = sizeof(host);
        int n = bind(_listensock, (struct sockaddr *)&host, hostlen);
        if (n == -1)
        {
            Logmessage(Fatal, "bind err ,error code %d,%s", errno, strerror(errno));
            exit(BING_ERR);
        }
    }

    int FD()
    {
        return _listensock;
    }
    ~Udp()
    {
        close(_listensock);
    }

protected:
    int _listensock;
    uint16_t _port;
};

class Tcp : public Udp
{
public:
    Tcp(uint16_t port)
        : Udp(port, TCP)
    {
    }

    Tcp()
        : Udp(TCP)
    {
    }

    void Listen()
    {

        int n = listen(_listensock, backlog);
        if (n == -1)
        {
            Logmessage(Fatal, "listen err ,error code %d,%s", errno, strerror(errno));
            exit(LISTEN_ERR);
        }
    }

    int Accept(string *clientip, uint16_t *clientport, int *err)
    {
        struct sockaddr_in client;
        socklen_t clientlen;
        int sock = accept(_listensock, (struct sockaddr *)&client, &clientlen);
        if (sock < 0)
        {
            Logmessage(Warning, "bind err ,error code %d,%s", errno, strerror(errno));
        }
        else
        {

            string tmp = inet_ntoa(client.sin_addr);
            *clientip = tmp;
            *clientport = ntohs(client.sin_port);
        }
        *err = errno;
        return sock;
    }

    void Connect(string ip, uint16_t port)
    {
        struct sockaddr_in server;
        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        server.sin_addr.s_addr = inet_addr(ip.c_str());
        socklen_t hostlen = sizeof(server);
        int n = connect(_listensock, (struct sockaddr *)&server, hostlen);
        if (n == -1)
        {
            Logmessage(Fatal, "Connect err ,error code %d,%s", errno, strerror(errno));
            exit(CONNECT_ERR);
        }
    }
    void Close()
    {
        close(_listensock);
    }

    ~Tcp()
    {
    }
};
