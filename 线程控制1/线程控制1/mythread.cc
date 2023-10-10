
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <pthread.h>
#include <unistd.h>

using namespace std;

// void *FuncRun1(void *argc)
// {
//     int count = 0;
//     while (1)
//     {
//         count++;
//         cout << "I am thread-1-count:" << count << endl;
//         sleep(1);
//         // 三秒后线程1退出
//         if (count == 3)
//         {
//             pthread_exit(NULL);
//         }
//     }
// }

// void *FuncRun2(void *argc)
// {
//     int count = 0;
//     while (1)
//     {
//         count++;
//         cout << "I am thread-2-count:" << count << endl;
//         sleep(1);
//         // 三秒后线程2退出
//         if (count == 3)
//         {
//             pthread_exit(NULL);
//         }
//     }
// }

// int main()
// {
//     // 线程id
//     pthread_t id1, id2;
//     // 创建线程
//     pthread_create(&id1, NULL, FuncRun1, NULL);
//     pthread_create(&id2, NULL, FuncRun2, NULL);

//     while (1)
//     {
//         cout << "I am main,my pid:" << getpid() << endl;
//         sleep(1);
//     }

//     return 0;
// }

// void *FuncRun1(void *argc)
// {
//     int count = 0;
//     while (1)
//     {
//         count++;
//         cout << "I am thread-1-count:" << count << endl;
//         sleep(1);
//     }
// }
// void *FuncRun2(void *argc)
// {
//     int count = 0;
//     while (1)
//     {
//         count++;
//         cout << "I am thread-2-count:" << count << endl;
//         sleep(1);
//     }
// }
// int main()
// {
//     // 线程id
//     pthread_t id1, id2;
//     // 创建线程
//     pthread_create(&id1, NULL, FuncRun1, NULL);
//     pthread_create(&id2, NULL, FuncRun2, NULL);
//     int count = 0;
//     while (1)
//     {
//         count++;
//         sleep(1);

//         if (count == 3)
//         {
//             // 三秒后终止线程
//             pthread_cancel(id1);
//             pthread_cancel(id2);
//         }
//         cout << "I am main" << endl;
//     }

//     return 0;
// }

void *FuncRun(void *argc)
{
    // 线程分离
    pthread_detach(pthread_self());
    int count = 0;
    while (1)
    {
        count++;
        cout << "I am thread-count:" << count << endl;
        sleep(1);
    }
}

int main()
{
    pthread_t tid;
    int n = pthread_create(&tid, NULL, FuncRun, NULL);
    if (n != 0)
    {
        cerr << "pthread_create:" << strerror(errno) << endl;
    }

    sleep(2);

    // 线程已经分离，再去线程等待，pthread_join会立即报错。
    if (pthread_join(tid, NULL) == 0)
    {
        printf("pthread wait success\n");
    }
    else
    {
        printf("pthread wait failed\n");
    }

    return 0;
}
