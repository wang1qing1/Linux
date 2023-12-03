#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <cassert>
#include <sys/types.h>
#include <sys/socket.h>

#include "Sock.hpp"
#include "Log.hpp"
#include "Epoll.hpp"
const static int timeout = 1000;
unordered_map<uint32_t, string> EventMap = {{EPOLLIN, "EPOLLIN"}, {EPOLLOUT, "EPOLLOUT"}};

class EpollServer
{
public:
    EpollServer(uint16_t port)
        : _port(port), _sock(port)
    {
        _sock.Bind();
        _sock.Listen();
        _epoll.Create();
        bool ret = _epoll.Addevent(_sock.FD(), EPOLLIN);
        assert(ret);
        (void)ret;
    }

    void Start()
    {
        while (1)
        {
            int n = _epoll.Wait(_eventarr, gsize, timeout);
            switch (n)
            {
            case 0: // 超时
                Logmessage(Debug, "timeout.....");
                break;
            case -1: // 出错
                Logmessage(Warning, "errno:%d,%s", errno, strerror(errno));
                break;
            default: // 有时间就绪
                HandleEvents(n);
                break;
            }
        }
    }

private:
    void Accept()
    {
        string clientip;
        uint16_t clientport;
        int fd = _sock.Accept(&clientip, &clientport);
        Logmessage(Debug, "%s:%d连接上服务器", clientip.c_str(), clientport);
        int ret = _epoll.Addevent(fd, EPOLLIN);
        assert(ret);
        (void)ret;
    }

    void Recv(int fd)
    {
        char buff[1024];
        int n = recv(fd, buff, sizeof(buff), 0);
        if (n > 0)
        {
            buff[n - 1] = 0;
            buff[n - 2] = 0;
            cout << "(-.-):" << buff << endl;
            string str = "(-_-)";
            string ret = str + buff + str;
            send(fd, ret.c_str(), ret.size(), 0);
        }
        else
        {
            if (n == 0) // 客户端关闭连接
            {
                Logmessage(Debug, "client the other side close ");
            }
            else if (n < 0) // 读取出错
            {
                Logmessage(Warning, "recv error,errno:%d,%s", errno, strerror(errno));
            }
            _epoll.Delevent(fd);
            close(fd);
        }
    }

    void HandleEvents(int n)
    {
        for (int i = 0; i < n; i++)
        {
            int fd = _eventarr[i].data.fd;
            uint64_t events = _eventarr[i].events;
            Logmessage(Debug, "Handle evnet,fd:%d,events:%s", fd, EventMap[events].c_str());
            if (events & EPOLLIN) // 都事件
            {
                if (fd == _sock.FD())
                {
                    Accept();
                }
                else
                {
                    Recv(fd);
                }
            }
            else if (events & EPOLLOUT) // 写事件
            {
                // TODO
            }
            else
            {
                // other
            }
        }
    }

private:
    uint16_t _port;
    Tcp _sock;
    Epoll _epoll;
    struct epoll_event _eventarr[gsize];
};
