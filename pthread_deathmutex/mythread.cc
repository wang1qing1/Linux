#include <iostream>
#include <unistd.h>
#include <pthread.h>

using namespace std;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *Run(void *args)
{
    pthread_mutex_lock(&mutex);
    cout << "I am thread" << endl;
    sleep(1);
    cout << "I am thread" << endl;
    sleep(1);
    cout << "I am thread" << endl;
    sleep(1);
    cout << "I am thread" << endl;
    sleep(1);

    pthread_mutex_lock(&mutex);

    cout << "I am thread,too." << endl;
    sleep(1);
    cout << "I am thread,too." << endl;
    sleep(1);
    cout << "I am thread,too." << endl;
    sleep(1);
    cout << "I am thread,too." << endl;
    sleep(1);
    return NULL;
}

int main()
{

    pthread_t tid;
    pthread_create(&tid, NULL, Run, NULL);

    int tmp = 0;
    while (1)
    {
        cin >> tmp;
        if (tmp == 1)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
    }
    sleep(20);

    return 0;
}