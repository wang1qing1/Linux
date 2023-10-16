#include "Thread_Pool.hpp"

int main()
{
    Thread_Pool<Task> pool;
    pool.Init();

    pool.Start();

    int a, b;
    char c;
    while (1)
    {
        cin >> a >> c >> b;
        Task task(a, b, c);
        pool.push_task(task);
    }
}