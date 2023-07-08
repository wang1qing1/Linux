#include "mythread.hpp"
#include "mutex.hpp"
int sticks = 10000;
pthread_mutex_t mutex;
void *run(void *args)
{
    string *s = (string *)args;
    while (1)
    {
        LockGuard mymutex(&mutex);
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
    Thread th2(222, run, &str);
    Thread th3(333, run, &str);
    th1.run();
    th2.run();
    th3.run();
    th1.join();
    th2.join();
    th3.join();

    return 0;
}
