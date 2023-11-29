#include <iostream>
#include <sys/select.h>
#include "Sock.hpp"
#include "Log.hpp"
using namespace std;

class Server
{
    typedef int type_t;
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
            _fdarr[i] = defaultfd;
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
            if (_fdarr[i] == defaultfd)
            {
                _fdarr[i] = fd;
                break;
            }
        }
        if (i == N)
        {
            close(fd);
            Logmessage(Error, "_fdarr[] is full!!!!!");
        }
    }

    void ServerIO(int fd, int index)
    {
        char buff[1024];
        int n = read(fd, buff, sizeof(buff));
        if (n > 0) // 读取成功
        {
            buff[n] = 0;
            cout << "client#:" << buff;
        }
        else if (n == 0) // 对端关闭
        {
            Logmessage(Info, "对端关闭");
            close(fd);
            _fdarr[index] = defaultfd;
        }
        else // 读取失败
        {
            Logmessage(Info, "读取失败,errno:%d,%s", errno, strerror(errno));
            close(fd);
            _fdarr[index] = defaultfd;
        }
    }

    void HeadleEvent(fd_set fdset)
    {
        // 这里说明已经有文件描述符就绪了,但是我们要区分到底是listen套接字，还是accept通信套接字
        // 已经就绪的文件描述符就在fdset里面
        // 遍历_fdarr,查看是否在fdset中
        for (int i = 0; i < N; i++)
        {
            if (_fdarr[i] != defaultfd)
            {
                if (_fdarr[i] == _tcplisten.FD() && FD_ISSET(_fdarr[i], &fdset)) // 就绪的文件描述符是listen套接字
                {
                    Accept();
                }
                else if (_fdarr[i] != _tcplisten.FD() && FD_ISSET(_fdarr[i], &fdset)) // 就绪的文件描述符是accept套接字
                {
                    ServerIO(_fdarr[i], i);
                }
            }
        }
    }

    void start()
    {
        _fdarr[0] = _tcplisten.FD();
        while (1)
        {
            // 让select对我们的listen套接字进行监测，知道有
            // 因为select，fdset是一个输入输出型参数,调用一次以后,select就不能知道之前有哪些文件描述符需要监测了
            fd_set fdset;
            FD_ZERO(&fdset);
            int maxfd = _fdarr[0];
            for (int i = 0; i < N; i++)
            {
                if (_fdarr[i] != defaultfd)
                {
                    FD_SET(_fdarr[i], &fdset);
                    maxfd = max(maxfd, _fdarr[i]);
                }
            }
            struct timeval select_time = {2, 0};
            int n = select(maxfd + 1, &fdset, nullptr, nullptr, &select_time);
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
                HeadleEvent(fdset);
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
            if (_fdarr[i] != defaultfd)
                cout << _fdarr[i] << " ";
        }
        cout << endl;
    }

private:
    uint16_t _port;
    Tcp _tcplisten;
    type_t _fdarr[N]; // 记录需要检测的文件描述符
};
