#pragma once
#include <iostream>
#include <string>
#include <pthread.h>
#include <functional>
#include "Sock.hpp"
#include "Protocol.hpp"

using fun_t = std::function<string(string &)>;
class Httpserver;
struct Args
{
    Args(Httpserver *ser, string ip, uint16_t port, int fd)
        : _ip(ip), _port(port), _pserver(ser), _fd(fd)
    {
    }
    int _fd;
    uint16_t _port;
    string _ip;
    Httpserver *_pserver;
};

class Httpserver
{
public:
    Httpserver(fun_t func, uint16_t port)
        : _func(func)
    {
        tcp = new Tcp(port);
        tcp->Bind();
        tcp->Listen();
        cout << "服务器创建成功" << endl;
    }
    void start()
    {
        while (1)
        {
            string clientip;
            uint16_t clientport;
            cout << "start accept" << endl;
            int sock = tcp->Accept(&clientip, &clientport);
            cout << "get a new connect" << endl;
            // 多线程处理请求
            pthread_t t;
            Args *args = new Args(this, clientip, clientport, sock);
            pthread_create(&t, nullptr, ThreadRun, args);
        }
    }

    ~Httpserver()
    {
        delete tcp;
    }

private:
    static void *ThreadRun(void *args)
    {
        pthread_detach(pthread_self());
        Args *ts = static_cast<Args *>(args);
        ts->_pserver->serverIO(ts->_fd);
        delete ts;
        return nullptr;
    }

    void serverIO(int fd)
    {

        string message;
        char buff[102400];
        // 1.确信,读取一个完整的http请求报文
        int n = recv(fd, buff, sizeof(buff), 0);
        message = buff;

        string re = _func(message);
        send(fd, re.c_str(), re.length(), 0);
        close(fd);
    }

private:
    Tcp *tcp;
    fun_t _func;
};