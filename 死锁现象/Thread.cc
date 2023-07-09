// 死锁
#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;
pthread_mutex_t mutex;
void *run(void *args)
{
    // pthread_mutex_t *mutex = (pthread_mutex_t *)args;
    pthread_mutex_lock(&mutex);
    cout << "我是一个线程,我得到一把锁" << endl;
    pthread_mutex_lock(&mutex);
    cout << "我又活过来了" << endl;
    return nullptr;
}
int main()
{
    pthread_t t1;

    // pthread_mutex_init(&mutex, nullptr);
    pthread_create(&t1, nullptr, run, nullptr);

    sleep(3);
    pthread_mutex_unlock(&mutex);
    // pthread_mutex_destroy(&mutex);

    return 0;
}
