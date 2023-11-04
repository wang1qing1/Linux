#pragma once
#include <iostream>
#include <string>
#include <pthread.h>
#include <functional>
#include "Sock.hpp"
#include "Protocol.hpp"

using fun_t = std::function<Responce(const Request &)>;
class server;
struct Args
{
    Args(server *ser, string ip, uint16_t port, int fd)
        : _ip(ip), _port(port), _pserver(ser), _fd(fd)
    {
    }
    int _fd;
    uint16_t _port;
    string _ip;
    server *_pserver;
};

class server
{
public:
    server(fun_t func, int port)
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
            int sock = tcp->Accept(&clientip, &clientport);
            cout << "get a new connect" << endl;
            // 多线程处理请求
            pthread_t t;
            Args *args = new Args(this, clientip, clientport, sock);
            pthread_create(&t, nullptr, ThreadRun, args);
        }
    }

    ~server()
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
    }

    void serverIO(int fd)
    {
        // 由于使用tcp面向数据流传输数据，所以我们并不能知道我们读取的是不是一个完整的报文。

        // 1.读取一个完整的请求报文
        string inputstr;
        string message;
        while (1)
        {
            int len = ReadRequest(fd, inputstr, &message);
            if (len == 0)
                continue;
            if (len < 0)
                break;
            // 2.去除报头——将报头和有效载荷分离
            message = (message, len);
            // 3.有效载荷反序列化
            Request request;
            request.deserialize(message);
            // 4.处理业务逻辑
            Responce responce = _func(request);
            // 5.有效载荷序列化
            message = responce.serialize();
            // 6.有效载荷添加报头
            message = Addhead(message);
            // 7.发送响应
            send(fd, message.c_str(), message.length(), 0);
        }
    }

private:
    Tcp *tcp;
    fun_t _func;
};