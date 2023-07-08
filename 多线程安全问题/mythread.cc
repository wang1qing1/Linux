// 抢票 多个线程共同争抢资源
#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdio>
using namespace std;

int ticket = 10000; // 票

void *run(void *arge)
{
    char *data = (char *)arge;
    while (ticket > 0)
    {
        usleep(2000);
        cout << data << ":get a ticket:" << ticket-- << endl;
    }
}
// 最终票数出现负数。
int main()
{

    pthread_t th[4];
    for (int i = 0; i < 4; i++)
    {
        char *data = new char[64];
        snprintf(data, 64, "thread-%d", i + 1);
        pthread_create(th + i, nullptr, run, data);
    }
    for (int i = 0; i < 4; i++)
    {
        pthread_join(th[i], nullptr);
    }
    while (1)
    {
        ;
    }
}
