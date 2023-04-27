#include<iostream>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
using namespace std;
//由数学运算产生的÷0，或者是野指针问题，都算得上是硬件产生的问题，
//他们也会产生信号。
void handler(int num)
{
    cout<<"have a signal ，the signal is "<< num <<endl;
    sleep(1);
}

int main()
{
    signal(8,handler);
    //1.除0
    // //Floating point exception(浮点异常),产生八号信号(SIGFPE)
    // int a=10;
    // a/=0;

    //2.野指针
    int *p=nullptr;
    p=(int*)0x12454135;
    //Segmentation faultd(段错误)(SIGSEGV)
    *p=100;
    //我们在C/C++当中除零，内存越界等异常，在系统层面上，是被当成信号处理的

    return 0;
}

// 1) SIGHUP	 2) SIGINT	 3) SIGQUIT	 4) SIGILL	 5) SIGTRAP
//  6) SIGABRT	 7) SIGBUS	 8) SIGFPE	 9) SIGKILL	10) SIGUSR1
// 11) SIGSEGV	12) SIGUSR2	13) SIGPIPE	14) SIGALRM	15) SIGTERM
// 16) SIGSTKFLT	17) SIGCHLD	18) SIGCONT	19) SIGSTOP	20) SIGTSTP
// 21) SIGTTIN	22) SIGTTOU	23) SIGURG	24) SIGXCPU	25) SIGXFSZ
// 26) SIGVTALRM	27) SIGPROF	28) SIGWINCH	29) SIGIO	30) SIGPWR
// 31) SIGSYS	
//信号


