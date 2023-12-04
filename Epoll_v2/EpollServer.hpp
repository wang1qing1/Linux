#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <cassert>
#include <sys/types.h>
#include <sys/socket.h>
#include "Sock.hpp"
#include "Log.hpp"
#include "Epoll.hpp"
#include "Util.hpp"
struct connection;
class EpollServer;
const static int timeout = 1000;
unordered_map<uint32_t, string> EventMap = {{EPOLLIN, "EPOLLIN"}, {EPOLLOUT, "EPOLLOUT"}};
using CallBack = std::function<void(connection *)>;

struct connection
{
    connection(int fd, uint32_t events)
        : _fd(fd), _event(events)
    {
    }

    void SetCallBack(CallBack recvfunc, CallBack sendfunc, CallBack excepfunc)
    {
        _recvfunc = recvfunc;
        _sendfunc = sendfunc;
        _excepfunc = excepfunc;
    }

    int _fd;         // 文件描述符
    uint32_t _event; // 关心的事件

    string _recvstr; // 接受缓冲区
    string _sendstr; // 发送缓冲区

    CallBack _recvfunc;  // 处理读取
    CallBack _sendfunc;  // 处理发送
    CallBack _excepfunc; // 处理异常

    // 客户端的信息
    string _clientip;
    uint16_t _port;
};

class EpollServer
{
public:
    EpollServer(uint16_t port)
        : _port(port), _sock(port)
    {
        _sock.Bind();
        _sock.Listen();
        _epoll.Create();
        AddConnection(_sock.FD(), EPOLLIN | EPOLLET);
    }

    void Distribution()
    {
        while (1)
        {
            LoopOnce();
        }
    }

    void LoopOnce()
    {
        // 1.提取就绪链接
        int n = _epoll.Wait(_eventarr, gsize, timeout);
        for (int i = 0; i < n; i++)
        {
            // 提取就绪链接信息
            int fd = _eventarr[i].data.fd;
            uint32_t event = _eventarr[i].events;
            Logmessage(Debug, "处理 fd:%d,events:%s", fd, EventMap[event]);
            // 2.处理就绪事件
            // 2.1将就绪的异常事件，转移到读写事件就绪
            if ((event & EPOLLERR) || (event & EPOLLHUP))
                event |= (EPOLLIN | EPOLLOUT);
            // 2.2 处理就绪读写事件
            if ((event & EPOLLIN) && ConnIsEXist(fd)) // 读事件就绪，处理
                _Connection[fd]->_recvfunc(_Connection[fd]);
            if ((event & EPOLLOUT) && ConnIsEXist(fd)) // 写事件就绪，处理
                _Connection[fd]->_recvfunc(_Connection[fd]);
        }
    }

    bool ConnIsEXist(int fd)
    {
        return _Connection.find(fd) != _Connection.end();
    }

    void AddConnection(int fd, uint32_t events, string clienip = "127.0.0.1", uint16_t port = 8081)
    {
        // 如果当前文件描述符是ET,文件描述符需要是非阻塞
        if (events | EPOLLET)
            Util::Noblock(fd);

        // 1.构建connection对象，添加到connection对象中
        connection *conn = new connection(fd, events);
        conn->_clientip = clienip;
        conn->_port = port;
        // 1.1设置回调,由于文件描述符的类型不同，设置的回调也会不同————listen和accept
        if (fd == _sock.FD()) // listen
        {
            conn->SetCallBack(bind(&EpollServer::Accept, this, placeholders::_1), nullptr, nullptr);
        }
        else // accept
        {
            conn->SetCallBack(bind(&EpollServer::Recv, this, placeholders::_1),
                              bind(&EpollServer::Send, this, placeholders::_1),
                              bind(&EpollServer::Execp, this, placeholders::_1));
        }
        // 1.1添加到connection对象中
        _Connection.insert({fd, conn});
        // 2.添加到内核Epoll对象
        _epoll.Addevent(fd, events);
    }

    void Accept(connection *conn)
    {
        do
        {
            int err;
            string clientip;
            uint16_t clientport;
            int fd = _sock.Accept(&clientip, &clientport, &err);

            if (fd > 0)
            {
                Logmessage(Debug, "%s:%d连接上服务器", clientip.c_str(), clientport);
                AddConnection(fd, EPOLLIN | EPOLLET, clientip, clientport);
            }
            else
            {
                if (err == EAGAIN || err == EWOULDBLOCK) // 可接收的链接已经处理完，没有可接受的链接了，导致出错
                    break;
                else if (err == EINTR) // 因信号中断导致链接接受错误
                    continue;
                else
                {
                    Logmessage(Warning, "errno:%d,%s", err, strerror(err));
                    continue;
                }
            }
        } while (conn->_event & EPOLLET);
    }

    void Recv(connection *conn)
    {
    }

    void Send(connection *conn)
    {
    }
    void Execp(connection *conn)
    {
    }

private:
    uint16_t _port;                               // 端口号
    Tcp _sock;                                    // 套接字对象
    Epoll _epoll;                                 // epoll对象
    struct epoll_event _eventarr[gsize];          // 就绪事件管理
    unordered_map<int, connection *> _Connection; // 当前事件的处理和缓冲区管理
};
