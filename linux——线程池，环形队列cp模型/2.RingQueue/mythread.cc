#include "RingQueue.hpp"

void *run_p(void *args)
{
    char ops[4] = {'+', '-', '*', '/'};
    RingQueue<Task> *RQ = static_cast<RingQueue<Task> *>(args);
    while (1)
    {
        int a = rand() % 100;
        int b = rand() % 100;
        int op = ops[(a * b) % 4];
        // cout << "111111111" << endl;
        Task tk(a, b, op);

        RQ->push(tk);
        tk.showtask();
        sleep(1);
    }
}
void *run_c(void *args)
{
    RingQueue<Task> *RQ = static_cast<RingQueue<Task> *>(args);
    while (1)
    {
        Task tk = RQ->pop();
        tk.run();
        tk.showresult();
        sleep(1);
    }
}

int main()
{
    RingQueue<Task> *RQ = new RingQueue<Task>(5);
    srand(time(0));
    pthread_t tid_c[5];
    pthread_t tid_p[5];

    pthread_create(&tid_c[0], nullptr, run_c, RQ);
    pthread_create(&tid_c[1], nullptr, run_c, RQ);
    pthread_create(&tid_c[2], nullptr, run_c, RQ);
    pthread_create(&tid_c[3], nullptr, run_c, RQ);
    pthread_create(&tid_c[4], nullptr, run_c, RQ);
    pthread_create(&tid_p[0], nullptr, run_p, RQ);
    pthread_create(&tid_p[1], nullptr, run_p, RQ);
    pthread_create(&tid_p[2], nullptr, run_p, RQ);
    pthread_create(&tid_p[3], nullptr, run_p, RQ);
    pthread_create(&tid_p[4], nullptr, run_p, RQ);

    pthread_join(tid_c[0], nullptr);
    pthread_join(tid_c[1], nullptr);
    pthread_join(tid_c[2], nullptr);
    pthread_join(tid_c[3], nullptr);
    pthread_join(tid_c[4], nullptr);
    pthread_join(tid_p[0], nullptr);
    pthread_join(tid_p[1], nullptr);
    pthread_join(tid_p[2], nullptr);
    pthread_join(tid_p[3], nullptr);
    pthread_join(tid_p[4], nullptr);
    delete RQ;

    return 0;
}