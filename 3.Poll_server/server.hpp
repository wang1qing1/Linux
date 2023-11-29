#include <iostream>
#include <sys/select.h>
#include <poll.h>

#include "Sock.hpp"
#include "Log.hpp"
using namespace std;

#define defaultevent 0

// struct pollfd
// {
//     int fd;        /* file descriptor */
//     short events;  /* requested events */
//     short revents; /* returned events */
// };

// POLLIN:There is data to read.
// POLLPRI:There is urgent data to read (e.g., out-of-band data on TCP socket; pseudoterminal master in packet mode  has
//         seen state change in slave).
// POLLOUT:Writing now will not block.
// POLLRDHUP:(since Linux 2.6.17)
//         Stream  socket peer closed connection, or shut down writing half of connection.  The _GNU_SOURCE feature test
//         macro must be defined (before including any header files) in order to obtain this definition.
// POLLERR:Error condition (output only).
// POLLHUP:Hang up (output only).
// POLLNVAL:Invalid request: fd not open (output only).

class Poll_Server
{
    typedef pollfd type_t;
    static const int N = 4096;
    static const int defaultfd = -1;

public:
    Poll_Server(uint16_t port)
        : _tcplisten(port), _port(port)
    {
        _tcplisten.Bind();
        _tcplisten.Listen();
        // 初始化
        for (int i = 0; i < N; i++)
        {
            _fdarr[i].fd = defaultfd;
            _fdarr[i].events = defaultevent;
            _fdarr[i].revents = defaultevent;
        }
    }

    // 处理连接
    void Accept()
    {
        // 1.接受连接
        string clientip;
        uint16_t clientport;
        int fd = _tcplisten.Accept(&clientip, &clientport);
        // 2.将新的accept连接放入fdarr,后续让select监测
        int i;
        for (i = 1; i < N; i++)
        {
            if (_fdarr[i].fd == defaultfd)
            {
                _fdarr[i].fd = fd;
                _fdarr[i].events = POLLIN;
                break;
            }
        }
        if (i == N)
        {
            close(fd);
            Logmessage(Error, "_fdarr[] is full!!!!!");
        }
    }

    void ServerIO(type_t fdevent, int index)
    {
        if (fdevent.revents & POLLIN)
        {
            char buff[1024];
            int n = read(fdevent.fd, buff, sizeof(buff));
            if (n > 0) // 读取成功
            {
                buff[n] = 0;
                cout << "client#:" << buff;

                // 返回一句话
                string str = "server#: I have message, ";
                str += buff;
                send(fdevent.fd, str.c_str(), str.size(), 0);
            }
            else if (n == 0) // 对端关闭
            {
                Logmessage(Info, "对端关闭");
                close(fdevent.fd);
                _fdarr[index].fd = defaultfd;
                _fdarr[index].events = defaultevent;
            }
            else // 读取失败
            {
                Logmessage(Info, "读取失败,errno:%d,%s", errno, strerror(errno));
                close(fdevent.fd);
                _fdarr[index].fd = defaultfd;
                _fdarr[index].events = defaultevent;
            }
        }
        else if (fdevent.revents & POLLOUT)
        {
            // TODO
        }
        else
        { // fdevent.event & EXCEP_EVENT
          // TODO
        }
    }

    void HeadleEvent()
    {
        // 这里说明已经有文件描述符就绪了,但是我们要区分到底是listen套接字，还是accept通信套接字
        // 已经就绪的文件描述符就在fdset里面
        // 遍历_fdarr,查看是否在fdset中
        for (int i = 0; i < N; i++)
        {
            if (_fdarr[i].fd != defaultfd)
            {
                if (_fdarr[i].fd == _tcplisten.FD() && _fdarr[i].revents & POLLIN) // 就绪的文件描述符是listen套接字
                {
                    Accept();
                }
                else if (_fdarr[i].fd != _tcplisten.FD() && _fdarr[i].revents & POLLIN || _fdarr[i].revents & POLLOUT) // 就绪的文件描述符是accept套接字
                {
                    ServerIO(_fdarr[i], i);
                }
            }
        }
    }

    void start()
    {
        _fdarr[0].fd = _tcplisten.FD();
        _fdarr[0].events = POLLIN;
        while (1)
        {
            // 让select对我们的listen套接字进行监测，知道有
            // 因为select，fdset是一个输入输出型参数,调用一次以后,select就不能知道之前有哪些文件描述符需要监测了

            int timeout = 1000;
            int n = poll(_fdarr, N, timeout);
            switch (n)
            {
            case 0: // 阻塞超时
                Logmessage(Debug, "time out...,errno:%d,%s", errno, strerror(errno));
                break;
            case -1: // 出现错误
                Logmessage(Warning, "errno:%d,%s", errno, strerror(errno));
                break;
            default: // 检测到有事件就绪
                Logmessage(Debug, "have a event really...");
                HeadleEvent();
                debug_fdarr();
                sleep(1);
                break;
            }
        }
    }

    void debug_fdarr()
    {
        cout << "_fdarr[]:";
        for (int i = 0; i < N; i++)
        {
            if (_fdarr[i].fd != defaultfd)
                cout << _fdarr[i].fd << " ";
        }
        cout << endl;
    }
    ~Poll_Server()
    {
        close(_tcplisten.FD());
    }

private:
    uint16_t _port;
    Tcp _tcplisten;
    type_t _fdarr[N]; // 记录需要检测的文件描述符
};
