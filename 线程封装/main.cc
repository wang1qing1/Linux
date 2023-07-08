#include "mythread.hpp"
int sticks = 10000;
void *run(void *args)
{
    string *s = (string *)args;
    while (1)
    {
        if (sticks > 0)
        {
            usleep(100);
            cout << "我是一个线程：" << *s << sticks-- << endl;
        }
        else
        {
            break;
        }
    }
}

int main()
{
    string str("hello world!");
    Thread th1(111, run, &str);
    th1.run();

    th1.join();
    return 0;
}
