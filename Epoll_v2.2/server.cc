#include <iostream>
#include "EpollServer.hpp"

// 请求处理函数，返回响应
Responce calculate(Request request)
{
    int result;
    int exitcode;
    switch (request._op)
    {
    case '+':
        result = request._x + request._y;
        exitcode = 0;
        break;
    case '-':
        result = request._x - request._y;
        exitcode = 0;
        break;
    case '*':
        result = request._x * request._y;
        exitcode = 0;
        break;
    case '/':
        if (request._y == 0)
            exitcode = 1;
        else
        {
            result = request._x / request._y;
            exitcode = 0;
        }
        break;
    case '%':
        if (request._y == 0)
            exitcode = 2;
        else
        {
            result = request._x % request._y;
            exitcode = 0;
        }
        break;
    default:
        break;
    }
    return Responce(result, exitcode);
}
int main()
{
    EpollServer epollserver(8080, calculate);
    epollserver.Distribution();

    return 0;
}