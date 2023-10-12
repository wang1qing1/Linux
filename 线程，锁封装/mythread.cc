#include "mythread.hpp"
#include "../mymutex.hpp/mutex.hpp"

int ticket = 1000;
// 动态；
pthread_mutex_t mutex;

// 静态
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *RobTicket(void *args)
{

    Thread *ts = static_cast<Thread *>(args);
    while (1)
    {
        {
            MutexGuard Lock(&mutex);
            if (ticket > 0)
            {
                usleep(2000);
                printf("%s-ticket,%d\n", ts->name().c_str(), ticket--);
            }
            else
            {
                break;
            }
        }

        usleep(100);
    }
}

int main()
{
    Thread ts1(1, RobTicket, &ts1);
    Thread ts2(2, RobTicket, &ts2);
    Thread ts3(3, RobTicket, &ts3);
    Thread ts4(4, RobTicket, &ts4);
    ts1.run();
    ts2.run();
    ts3.run();
    ts4.run();

    ts1.join();
    ts2.join();
    ts3.join();
    ts4.join();

    return 0;
}
