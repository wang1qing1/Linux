
#include <iostream>
#include <cstdio>
#include <ctime>
#include <cstdlib>

struct Task
{
    Task(int a = 1, int b = 1, char op = '+')
        : _a(a), _b(b), _op(op)
    {
    }

    void run()
    {
        switch (_op)
        {
        case '+':
            _ret = _a + _b;
            break;
        case '-':
            _ret = _a - _b;
            break;
        case '*':
            _ret = _a * _b;
            break;
        case '/':
            if (_b == 0)
            {
                _exitcode = -1;
                break;
            }
            _ret = _a / _b;
            break;
        default:
            break;
        }
    }

    void showtask()
    {
        printf("producer:%d %c %d = ?\n", _a, _op, _b);
    }

    void showresult()
    {
        printf("consumer:%d %c %d = %d(exitcode:%d)\n", _a, _op, _b, _ret, _exitcode);
    }

    ~Task() {}

private:
    int _a;
    int _b;
    char _op;
    int _ret;

    int _exitcode = 0;
};
