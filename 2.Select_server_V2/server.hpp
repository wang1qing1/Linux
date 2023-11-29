#include <iostream>
#include <sys/select.h>
#include "Sock.hpp"
#include "Log.hpp"
using namespace std;

#define READ_EVENT (1 << 0)
#define WRITE_EVENT (1 << 1)
#define EXCEP_EVENT (1 << 2)

struct FdEvent
{
    int fd;
    uint16_t event;
    string clientip;
    uint16_t clientport;
};

class Server
{
    typedef FdEvent type_t;
    static const int N = (sizeof(fd_set) * 8);
    static const int defaultfd = -1;

public:
    Server(uint16_t port)
        : _tcplisten(port), _port(port)
    {
        _tcplisten.Bind();
        _tcplisten.Listen();
        // 初始化
        for (int i = 0; i < N; i++)
        {
            _fdarr[i].fd = defaultfd;
            _fdarr[i].event = 0;
            _fdarr[i].clientport = -1;
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
                _fdarr[i].clientip = clientip;
                _fdarr[i].clientport = clientport;
                _fdarr[i].event = READ_EVENT;
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
        if (fdevent.event & READ_EVENT)
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
                _fdarr[index].event = 0;
                _fdarr[index].clientport = 0;
                _fdarr[index].clientip = "";
            }
            else // 读取失败
            {
                Logmessage(Info, "读取失败,errno:%d,%s", errno, strerror(errno));
                close(fdevent.fd);
                _fdarr[index].fd = defaultfd;
                _fdarr[index].event = 0;
                _fdarr[index].clientport = 0;
                _fdarr[index].clientip = "";
            }
        }
        else if (fdevent.event & WRITE_EVENT)
        {
            // TODO
        }
        else
        { // fdevent.event & EXCEP_EVENT
          // TODO
        }
    }

    void HeadleEvent(fd_set fdset_read, fd_set fdset_write)
    {
        // 这里说明已经有文件描述符就绪了,但是我们要区分到底是listen套接字，还是accept通信套接字
        // 已经就绪的文件描述符就在fdset里面
        // 遍历_fdarr,查看是否在fdset中
        for (int i = 0; i < N; i++)
        {
            if (_fdarr[i].fd != defaultfd)
            {
                if (_fdarr[i].fd == _tcplisten.FD() && FD_ISSET(_fdarr[i].fd, &fdset_read)) // 就绪的文件描述符是listen套接字
                {
                    Accept();
                }
                else if (_fdarr[i].fd != _tcplisten.FD() && FD_ISSET(_fdarr[i].fd, &fdset_read) || FD_ISSET(_fdarr[i].fd, &fdset_write)) // 就绪的文件描述符是accept套接字
                {
                    ServerIO(_fdarr[i], i);
                }
            }
        }
    }

    void start()
    {
        _fdarr[0].fd = _tcplisten.FD();
        _fdarr[0].event = READ_EVENT;
        while (1)
        {
            // 让select对我们的listen套接字进行监测，知道有
            // 因为select，fdset是一个输入输出型参数,调用一次以后,select就不能知道之前有哪些文件描述符需要监测了
            fd_set fdset_read;
            FD_ZERO(&fdset_read);
            fd_set fdset_write;
            FD_ZERO(&fdset_write);
            int maxfd = _fdarr[0].fd;
            for (int i = 0; i < N; i++)
            {
                if (_fdarr[i].fd != defaultfd)
                {
                    if (_fdarr[i].event & READ_EVENT)
                        FD_SET(_fdarr[i].fd, &fdset_read);
                    else if (_fdarr[i].event & WRITE_EVENT)
                        FD_SET(_fdarr[i].fd, &fdset_write);

                    maxfd = max(maxfd, _fdarr[i].fd);
                }
            }
            struct timeval select_time = {2, 0};
            int n = select(maxfd + 1, &fdset_read, &fdset_write, nullptr, &select_time);
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
                HeadleEvent(fdset_read, fdset_write);
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

private:
    uint16_t _port;
    Tcp _tcplisten;
    type_t _fdarr[N]; // 记录需要检测的文件描述符
};
