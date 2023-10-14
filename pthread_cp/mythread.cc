#include "BlockQueue.hpp"

using namespace std;

struct Task
{
    Task(int a, int b, char op)
        : _a(a), _b(b), _op(op)
    {
    }

    char _op;
    int _a;
    int _b;
    int ret;
    int _exitcode;
};

void *Run1(void *args)
{
    BlockQueue<Task> *pBQ = static_cast<BlockQueue<Task> *>(args);
    while (1)
    {
        char op_arr[4] = {'+', '-', '*', '/'};
        int a = rand() % 10;
        int b = rand() % 10;
        char op = op_arr[(a * b) % 4];
        Task tk(a, b, op);
        pBQ->push(tk);
        printf("%d %c %d =u ?,task-account:%d\n", a, op, b, pBQ->_q.size());
        usleep(50000);
    }

    return NULL;
}

void *Run2(void *args)
{
    BlockQueue<Task> *pBQ = static_cast<BlockQueue<Task> *>(args);

    while (1)
    {

        Task tk = pBQ->pop();
        switch (tk._op)
        {
        case '+':
            tk.ret = tk._a + tk._b;
            break;
        case '-':
            tk.ret = tk._a - tk._b;
            break;
        case '*':
            tk.ret = tk._a * tk._b;
            break;
        case '/':
            if (tk._b == 0)
            {
                exit(-1);
            }
            tk.ret = tk._a / tk._b;
            break;
        default:
            break;
        }
        printf("%d %c %d = %d\n", tk._a, tk._op, tk._b, tk.ret);
        sleep(1);
    }

    return NULL;
}

int main()
{
    BlockQueue<Task> BQ(5);
    pthread_t tid[2];
    srand(time(nullptr));
    pthread_create(&tid[0], NULL, Run1, &BQ);
    pthread_create(&tid[1], NULL, Run2, &BQ);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    return 0;
}