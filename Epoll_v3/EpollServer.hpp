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
#include "Protocol.hpp"
struct connection;
class EpollServer;
const static int timeout = -1;
unordered_map<uint32_t, string> EventMap = {{EPOLLIN, "EPOLLIN"}, {EPOLLOUT, "EPOLLOUT"}};
using CallBack = std::function<void(connection *)>;
using Func = std::function<Responce(Request)>;

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
    EpollServer(uint16_t port, Func func)
        : _port(port), _sock(port), _func(func)
    {
        _sock.Bind();
        _sock.Listen();
        _epoll.Create();
        AddConnection(_sock.FD(), EPOLLIN | EPOLLET);
        Logmessage(Debug, "init server success");
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
            // 2.处理就绪事件
            // 2.1将就绪的异常事件，转移到读写事件就绪
            if ((event & EPOLLERR) || (event & EPOLLHUP))
                event |= (EPOLLIN | EPOLLOUT | EPOLLET);
            // 2.2 处理就绪读写事件
            if ((event & EPOLLIN) && ConnIsEXist(fd)) // 读事件就绪，处理
            {
                _Connection[fd]->_recvfunc(_Connection[fd]);
                // Logmessage(Debug, "处理 fd:%d,events:%s,的读事件", fd, EventMap[event].c_str());
            }
            if ((event & EPOLLOUT) && ConnIsEXist(fd)) // 写事件就绪，处理
            {
                _Connection[fd]->_sendfunc(_Connection[fd]);
                // Logmessage(Debug, "处理 fd:%d,events:%s,的写事件", fd, EventMap[event].c_str());
            }
        }
    }

    bool ConnIsEXist(int fd)
    {
        return _Connection.find(fd) != _Connection.end();
    }

    void AddConnection(int fd, uint32_t events, string clienip = "127.0.0.1", uint16_t port = 8081)
    {
        // 如果当前文件描述符是ET,文件描述符需要是非阻塞
        if (events & EPOLLET)
            Util::Noblock(fd);

        // 1.构建connection对象，添加到connection对象中
        connection *conn = new connection(fd, events);
        conn->_clientip = clienip;
        conn->_port = port;
        // 1.1设置回调,由于文件描述符的类型不同，设置的回调也会不同————listen和accept
        if (fd == _sock.FD()) // listen
        {
            conn->SetCallBack(std::bind(&EpollServer::Accept, this, placeholders::_1), nullptr, nullptr);
        }
        else // accept
        {
            conn->SetCallBack(std::bind(&EpollServer::Recv, this, placeholders::_1),
                              std::bind(&EpollServer::Send, this, placeholders::_1),
                              std::bind(&EpollServer::Execp, this, placeholders::_1));
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
            int err = 0;
            string clientip;
            uint16_t clientport;
            int fd = _sock.Accept(&clientip, &clientport, &err);
            if (fd > 0)
            {
                Logmessage(Debug, "有一个client:IP->%s,client:port->%d连接上服务器", clientip.c_str(), clientport);
                AddConnection(fd, EPOLLIN | EPOLLET, clientip, clientport);
            }
            else
            {
                if (err == EAGAIN || err == EWOULDBLOCK) // 在非阻塞的时候，可接收的链接已经处理完，没有可接受的链接了，导致出错
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

    void ProtocolHandle(connection *conn)
    {
        bool quit = true;
        string request_str;
        while (1)
        {
            // 判断是否读取到一个完整的报文,返回有效载荷长度
            int len = ReadFormat(conn->_recvstr, &request_str);
            if (len == 0)
                continue;
            // 读取到一个完整的报文————request,开始处理报文
            // 1.去报头
            request_str = Rehead(request_str, len);
            // 2.构建请求
            Request request;
            // 3.反序列化
            request.deserialize(request_str);
            // 4.处理业务
            Responce responce = _func(request);
            // 5.序列化
            string responce_str = responce.serialize();
            // 6.添加报头
            responce_str = Addhead(responce_str);
            // 7.添加到发送缓冲区
            conn->_sendstr += responce_str;
            // 8.开启对write 事件的关心
            EnableReadWrite(conn, true, true);
            break;
        }
    }
    bool RecvHandle(connection *conn)
    {
        bool stat;
        // 读取一个完整的报文,根据自己协议定制的
        char buff[1024];
        int fd = conn->_fd;
        do
        {
            // 尝试读取
            int n = recv(conn->_fd, buff, sizeof(buff) - 1, 0);
            if (n > 0) // 正确读取到数据
            {
                // 添加到独立缓冲区
                buff[n] = 0;
                conn->_recvstr += buff;
            }
            else if (n == 0)
            { // 对端关闭
                conn->_excepfunc(conn);
                stat = false;
                break;
            }
            else
            {
                // 读取出错
                if (errno == EAGAIN || errno == EWOULDBLOCK) // 1.在非阻塞时候，没有数据时读取
                    break;
                else if (errno == EINTR) // 读取时信号中断
                    continue;
                else // 读取异常
                {
                    conn->_excepfunc(conn);
                    stat = false;
                    break;
                }
            }
        } while (conn->_event & EPOLLET);
    }

    void Recv(connection *conn)
    {
        Logmessage(Debug, "处理 fd:%d,events:%s,的读事件", conn->_fd, "EPOLLIN");
        // 报文读取
        int stat = RecvHandle(conn);
        if (!stat)
            return;
        // 报文协议解析
        ProtocolHandle(conn);
    }

    bool EnableReadWrite(connection *conn, bool readable, bool writeable)
    {
        conn->_event = (readable ? EPOLLIN : 0) | (writeable ? EPOLLOUT : 0) | EPOLLET;
        return AddModEvent(conn->_fd, conn->_event, EPOLL_CTL_MOD);
    }

    bool AddModEvent(int fd, uint32_t event, int op)
    {
        struct epoll_event ev;
        ev.data.fd = fd;
        ev.events = event;
        int n = epoll_ctl(_epoll.Fd(), op, fd, &ev);
        if (n < 0)
        {
            Logmessage(Warning, "epoll_ctl error, code: %d, errstring: %s", errno, strerror(errno));
            return false;
        }
        return true;
    }

    void Send(connection *conn)
    {
        Logmessage(Debug, "处理 fd:%d,events:%s,的写事件", conn->_fd, "EPOLLOUT");
        do
        {
            int n = send(conn->_fd, conn->_sendstr.c_str(), conn->_sendstr.size(), 0);
            if (n > 0)
            {
                conn->_sendstr.erase(0, n);
                if (conn->_sendstr.empty())
                {
                    cout << "send end..." << endl;
                    EnableReadWrite(conn, true, false);
                    break;
                }
                else
                {
                    cout << "send no end..." << endl;
                    EnableReadWrite(conn, true, true);
                }
            }
            else
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    Logmessage(Warning, "Send:errno:%s,%s", "EAGAIN & EWOULDBLOCK", strerror(errno));
                    break;
                }
                else if (errno == EINTR)
                {
                    Logmessage(Warning, "Send:errno:%s,%s", "EINTR", strerror(errno));
                    continue;
                }
                else
                {
                    conn->_excepfunc(conn);
                    Logmessage(Warning, "Send:errno:%s,%s", errno, strerror(errno));
                    break;
                }
            }
        } while (conn->_event & EPOLLET);
    }
    void Execp(connection *conn)
    {
        Logmessage(Debug, "Excepter..., fd: %d, clientinfo: [%s:%d]", conn->_fd, conn->_clientip.c_str(), conn->_port);

        // 1.从epoll中去除
        _epoll.Delevent(conn->_fd);

        // 2.关闭文件描述符
        close(conn->_fd);

        // 3.从_Connection中去除
        _Connection.erase(conn->_fd);

        // 4.释放connection对象
        delete conn;
    }

private:
    uint16_t _port;                               // 端口号
    Tcp _sock;                                    // 套接字对象
    Epoll _epoll;                                 // epoll对象
    struct epoll_event _eventarr[gsize];          // 就绪事件管理
    unordered_map<int, connection *> _Connection; // 当前事件的处理和缓冲区管理
    Func _func;                                   // 逻辑处理函数
};
