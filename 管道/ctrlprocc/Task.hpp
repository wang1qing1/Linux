#include <vector>
#include <iostream>
#include <unistd.h>

using namespace std;
typedef void (*fun_t)();

void beatxyy()
{
    cout << "子进程：" << getpid() << ",暴打徐洋洋" << endl;
}

void beatxyf()
{
    cout << "子进程：" << getpid() << ",暴打谢一凡" << endl;
}

void beatwy()
{
    cout << "子进程：" << getpid() << ",暴打吴悠" << endl;
}

void beatwj()
{
    cout << "子进程：" << getpid() << ",暴打吴舰" << endl;
}

void beatxjy()
{
    cout << "子进程：" << getpid() << ",暴打许吉意" << endl;
}

struct Task
{
    Task()
    {
        funcs.push_back(beatxyy);
        funcs.push_back(beatxyf);
        funcs.push_back(beatwy);
        funcs.push_back(beatwj);
        funcs.push_back(beatxjy);
    }

    vector<fun_t> funcs;
};