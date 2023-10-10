#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>

using namespace std;

// void *FuncRun(void *argc)
// {
//     while (1)
//     {
//         cout << "I am thread,my pid:" << getpid() << endl;
//         sleep(1);
//     }
// }

// int main()
// {
//     // 线程id
//     pthread_t id;
//     // 创建线程
//     pthread_create(&id, NULL, FuncRun, NULL);

//     while (1)
//     {
//         cout << "I am main,my pid:" << getpid() << endl;
//         sleep(1);
//     }

//     return 0;
// }

// // 任务，计算[1-top]的求和，并将结果存储到sum中。
// struct task
// {
//     task(int top, int num)
//         : _thread_name("thread" + to_string(num)), _top(top), _sum(0), _num(num)
//     {
//     }

//     string _thread_name; // 线程名字
//     int _top;            // 计算数据范围
//     int _sum;            // 结果
//     int _num;            // 线程编号
// };

// // 线程函数
// void *FuncRun(void *argc)
// {
//     task *t = (task *)argc;
//     for (int i = 1; i <= t->_top; i++)
//     {
//         t->_sum += i;
//     }
// }

// int main()
// {
//     // 线程id
//     pthread_t id1, id2;
//     // 创建线程

//     task t1(100, 1);
//     task t2(150, 2);

//     pthread_create(&id1, NULL, FuncRun, &t1);
//     pthread_create(&id2, NULL, FuncRun, &t2);
//     // 等待线程计算完再输出结果
//     sleep(1);

//     cout << t1._thread_name << ":[1-" << t1._top << "]=" << t1._sum << endl;
//     cout << t2._thread_name << ":[1-" << t2._top << "]=" << t2._sum << endl;

//     return 0;
// }

void *FuncRun1(void *argc)
{
    int *top = (int *)argc;
    int *sum = new int;
    for (int i = 1; i <= *top; i++)
    {
        *sum += i;
    }
    // 线程退出
    pthread_exit(sum);
}

void *FuncRun2(void *argc)
{
    int *top = (int *)argc;
    int *sum = new int;
    for (int i = 1; i <= *top; i++)
    {
        *sum += i;
    }
    // 线程退出
    return sum;
}

void *FuncRun3(void *argc)
{
    int *top = (int *)argc;
    int *sum = new int;
    for (int i = 1; i <= *top; i++)
    {
        *sum += i;
        sleep(1);
    }
    free(sum);
    // 线程退出
}

int main()
{

    int top1 = 100;
    int top2 = 150;
    int top3 = 200;

    pthread_t id1;
    pthread_t id2;
    pthread_t id3;

    pthread_create(&id1, NULL, FuncRun1, &top1);
    pthread_create(&id2, NULL, FuncRun2, &top2);
    pthread_create(&id3, NULL, FuncRun3, &top3);
    pthread_cancel(id3);

    // 接受线程返回数据
    void *ret_ptr1;
    void *ret_ptr2;
    void *ret_ptr3;
    // 等待线程
    pthread_join(id1, &ret_ptr1);
    pthread_join(id2, &ret_ptr2);
    pthread_join(id3, &ret_ptr3);

        cout << "ret1:" << *((int *)ret_ptr1) << endl;
    free(ret_ptr1);
    cout << "ret2:" << *((int *)ret_ptr2) << endl;
    free(ret_ptr2);
    if (ret_ptr3 == PTHREAD_CANCELED)
        cout << "ret3:PTHREAD_CANCELED" << endl;

    return 0;
}