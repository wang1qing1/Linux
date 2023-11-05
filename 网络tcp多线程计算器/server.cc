#include "Server.hpp"
#include "Log.hpp"
#include "Protocol.hpp"

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
    server Server(calculate, 8080);
    Server.start();

    return 0;
}
