#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>

#define LOG "ab.txt"
#define LOG1 "log.txt"

void test()
{
    umask(0);
     //返回值是一个整数--文件描述符。
     close(0);
    int fd=open(LOG,O_RDONLY);
    close(1);
    int fd1=open(LOG1,O_WRONLY|O_CREAT,0666);


    //一个进程被加载以后，会默认打开三个文件
    //1.标准输入----stdin(C语言)
    //2.标准输出----stdout(C语言)
    //3.标准错误----stderr(C语言)
    //文件描述符是什么？
    //内存中存在着大量的文件，这些文件需要被操作系统，统一的管理起来，所以操作系统1对每一个加载到内存的文件做了一种描述。
    //这种描述类似于OS对进程的描述(PCB),所以操作系统也有对每一个加载到内存的文件描述---struct_file.
    //struct_file会包含文件的各种属性，以及文件的缓冲区等。
    //这些结构在内存中，会被使用链表组织起来。当我们对内存中某一个文件进程增删查改时，就是对链表的增删查改。
    //但是一个文件被打开，为什么要加载到内存?由于冯诺依曼体系结构的限制。
    //一个文件被打开，在我们当前看来，是用户使用某一个进程，让操作系统打开了一个文件(将文件加载到内存)，操作一个文件，归根结底也是一个进程对文件的操作。
    //所以OS必须维护一种进程与文件系统的关系，以便进程对文件的读写操作。而进程与文件系统交互的媒介就是文件描述符。
    //文件描述符具体是什么？
    //首先，既然一个文件的打开归根结底是进程打开的。那么在进程PCB中一定会有关于该进程打开的文件的管理结构。
    //1.在PCB中会存在一个类似于：struct_file* file[];这是一个指针数组。每一个元素，都是一个struct_file*。每一个指针都指向了一个文件描述的结构体。
    //2.而这个数组的下标就是该进程的文件描述符。
    //3.上述讲的，一个进程被加载以后，会默认打开三个文件，他们就是struct_file* file[]，的前三个元素。他们的文件描述符也就是：0，1，2.
    //验证：
    int a;
    int b;
    scanf("%d%d",&a,&b);
    printf("a=%d,b=%d",a,b);


}
void test1()
{
    //1.标准输入----stdin(C语言)
    //2.标准输出----stdout(C语言)
    //3.标准错误----stderr(C语言)
    //他们的文件描述符也就是：0，1，2.
    int fd1=open(LOG1,O_WRONLY|O_CREAT,0666);
    printf("stdin:%d\n",stdin->_fileno);
    printf("stdout:%d\n",stdout->_fileno);
    printf("stderr:%d\n",stderr->_fileno);
    printf("LOG:%d\n",fd1);
}

void test2()
{
    //重定向问题:
    printf("stdout,hello world\n");
    printf("stdout,hello world\n");
    printf("stdout,hello world\n");
    printf("stdout,hello world\n");
    printf("stdout,hello world\n");
    perror("stderr,hello world");
    perror("stderr,hello world");
    perror("stderr,hello world");
    perror("stderr,hello world");
    perror("stderr,hello world");
}

int main()
{
    //test();
   // test1();
    test2();


    return 0;
}