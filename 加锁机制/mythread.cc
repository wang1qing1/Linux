// 抢票 多个线程共同争抢资源
#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdio>
using namespace std;

int ticket = 10000;    // 票
pthread_mutex_t mutex; // 锁
void *run(void *arge)
{
    char *data = (char *)arge;
    while (1)
    {
        pthread_mutex_lock(&mutex);
        if (ticket > 0)
        {
            usleep(20);
            cout << data << ":get a ticket:" << ticket-- << endl;
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            break;
            pthread_mutex_unlock(&mutex);
        }
    }
}
// 最终票数出现负数。
int main()
{
    pthread_mutex_init(&mutex, nullptr); // 初始化锁
    pthread_t th[4];
    for (int i = 0; i < 4; i++)
    {
        char *data = new char[64];
        snprintf(data, 64, "thread-%d", i + 1);
        // 创建线程
        pthread_create(th + i, nullptr, run, data);
    }
    for (int i = 0; i < 4; i++)
    {
        pthread_join(th[i], nullptr);
    }
    pthread_mutex_destroy(&mutex);

    while (1)
    {
        ;
    }
}
