#include <iostream>
#include <cstring>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
using namespace std;

void SetNoBlock(int fd)
{
    int fl = fcntl(fd, F_GETFL);
    if (fl < 0)
    {
        perror("fcntl");
        return;
    }
    fcntl(fd, F_SETFL, fl | O_NONBLOCK);
}
// int main()
// {
//     // 将标准输入设置成非阻塞
//     SetNoBlock(0);
//     char buff[1024];
//     // 循环读取
//     while (1)
//     {
//         int n = read(0, buff, sizeof(buff));
//         if (n < 0)
//         {
//             if (errno == EAGAIN || errno == EWOULDBLOCK)
//             {
//                 printf("No Data,errno:%d,%s\n", errno, strerror(errno));
//             }
//             sleep(1);
//             continue;
//         }
//         cout << "read Data:" << buff << endl;
//     }
// }

// /* The fd_set member is required to be an array of longs.  */
// typedef long int __fd_mask;
// /* fd_set for select and pselect.  */
// #define __FD_SETSIZE 1024
// #define __NFDBITS (8 * (int)sizeof(__fd_mask))
// typedef struct
// {
//     /* XPG4.2 requires this member name.  Otherwise avoid the name
//        from the global namespace.  */
// #ifdef __USE_XOPEN
//     __fd_mask fds_bits[__FD_SETSIZE / __NFDBITS]; //
// #define __FDS_BITS(set) ((set)->fds_bits)
// #else
//     __fd_mask __fds_bits[__FD_SETSIZE / __NFDBITS];
// #define __FDS_BITS(set) ((set)->__fds_bits)
// #endif
// } fd_set;

int main()
{
    fd_set fdset;
    cout << sizeof(fd_set) * 8 << endl;
}
