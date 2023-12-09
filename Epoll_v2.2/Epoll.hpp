#pragma once
#include <iostream>
#include <vector>
#include <string.h>
#include <sys/epoll.h>
#include "Log.hpp"

using namespace std;
const static int gsize = 128;
const static int defaultepfd = -1;

class Epoll
{
public:
    Epoll()
    {
    }

    void Create(int size = gsize)
    {
        _epfd = epoll_create(size);
        if (_epfd < 0)
        {
            Logmessage(Error, "errno:%d,%s", errno, strerror(errno));
            exit(Error);
        }
    }

    // typedef union epoll_data
    // {
    //     void *ptr;
    //     int fd;
    //     uint32_t u32;
    //     uint64_t u64;
    // } epoll_data_t;

    // struct epoll_event
    // {
    //     uint32_t events;   /* Epoll events */
    //     epoll_data_t data; /* User data variable */
    // };

    bool Addevent(int fd, uint32_t events)
    {
        epoll_event event;
        event.events = events;
        event.data.fd = fd;
        int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &event);
        if (ret < 0)
        {
            Logmessage(Warning, "errno:%d,%s", errno, strerror(errno));
            return false;
        }
        return true;
    }

    bool Delevent(int fd)
    {
        int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, nullptr);
        if (ret < 0)
        {
            Logmessage(Warning, "errno:%d,%s", errno, strerror(errno));
            return false;
        }
        return true;
    }

    bool Modevent(int fd, uint32_t events)
    {
        epoll_event event;
        event.data.fd = fd;
        event.events = events;
        int ret = epoll_ctl(_epfd, EPOLL_CTL_MOD, fd, &event);
        if (ret < 0)
        {
            Logmessage(Warning, "errno:%d,%s", errno, strerror(errno));
            return false;
        }
        return true;
    }

    int Wait(struct epoll_event *eventarr, int maxevent, int timeout)
    {
        // int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
        int n = epoll_wait(_epfd, eventarr, maxevent, timeout);
        return n;
    }

    int Fd()
    {
        return _epfd;
    }

    void Close()
    {
        if (_epfd != defaultepfd)
            close(_epfd);
    }

    ~Epoll()
    {
    }

private:
    int _epfd = defaultepfd;
};