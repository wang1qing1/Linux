#include <pthread.h>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <iostream>

using namespace std;

pthread_cond_t cond;
pthread_mutex_t mutex;
void *run(void *args)
{
    char *name = (char *)args;
    while (1)
    {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        cout << name << ":活动" << endl;
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    pthread_t tid[4];
    for (int i = 0; i < sizeof(tid) / sizeof(tid[0]); i++)
    {
        char *name = new char[32];
        snprintf(name, 32, "pthread->%d", i + 1);
        pthread_create(tid + i, nullptr, run, name);
    }

    sleep(3);
    while (1)
    {
        cout << "主线程runging......" << endl;

        pthread_cond_broadcast(&cond);

        sleep(1);
    }
    for (int i = 0; i < sizeof(tid) / sizeof(tid[0]); i++)
    {
        pthread_join(tid[i], nullptr);
    }

    return 0;
}