#include <iostream>
#include <unistd.h>
#include <pthread.h>
using namespace std;
int count = 0;
void *run1(void *arge)
{
    while (true)
    {
        cout << "我是线程1"
             << ":" << count++ << endl;
        sleep(1);
    }
}
void *run2(void *arge)
{
    while (true)
    {
        cout << "我是线程2"
             << ":" << count++ << endl;
        sleep(1);
    }
}

int main()
{
    pthread_t t1, t2;
    pthread_create(&t1, nullptr, run1, nullptr);
    pthread_create(&t2, nullptr, run2, nullptr);
    while (1)
    {
    }
    return 0;
}