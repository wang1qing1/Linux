// #include <stdio.h>
// #include <string.h>
// int main()
// {
//     FILE *fp = fopen("myfile", "w");
//     if (!fp)
//     {
//         printf("fopen error!\n");
//     }
//     const char *msg = "hello Linux!\n";
//     const char *msg2 = "hello C++!\n";
//     int count = 5;
//     while (count--)
//     {
//         // 向文件中写入，
//         // 参数1：写入的数据C++
//         // 参数2：写入的字符个数
//         // 参数3：写入的数据元素的个数
//         // 参数4：写入的文件结构体指针
//         fwrite(msg, strlen(msg), 1, fp);
//     }
//     int n = 5;
//     while (n--)
//     {
//         // 向文件中写入，
//         // 参数1：写入的文件结构体指针
//         // 参数2：格式化写入
//         fprintf(fp, "[%d]:%s", n, msg2);
//     }
//     fclose(fp);
//     return 0;
// }

// #include <stdio.h>
// #include <string.h>
// int main()
// {
//     FILE *fp = fopen("myfile", "r");
//     if (!fp)
//     {
//         printf("fopen error!\n");
//     }
//     char buf[1024];
//     const char *msg = "hello bit!\n";
//     while (1)
//     {
//         // 注意返回值和参数，此处有坑，仔细查看man手册关于该函数的说明
//         size_t s = fread(buf, 1, strlen(msg), fp);
//         if (s > 0)
//         {
//             buf[s] = 0;
//             printf("%s", buf);
//         }
//         if (feof(fp))
//         {
//             break;
//         }
//     }
//     fclose(fp);
//     return 0;
// }

// #include <stdio.h>
// #include <string.h>
// int main()
// {
//     const char *msg = "hello fwrite\n";
//     // 1.fwrite
//     fwrite(msg, strlen(msg), 1, stdout);
//     // 2.printf
//     printf("hello printf\n");
//     // 3.fprintf
//     fprintf(stdout, "hello fprintf\n");
//     return 0;

//     // // 1.默认向显示器格式化打印
//     // int printf(const char *format, ...);
//     // // 2.向指定的文件中格式化输入
//     // int fprintf(FILE * stream, const char *format, ...);
//     // // 3.向指定的空间中格式化输入
//     // int sprintf(char *str, const char *format, ...);
//     // // 4.向指定的空间中格式化输入指定个数字符
//     // int snprintf(char *str, size_t size, const char *format, ...);
// }

// #include <stdio.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>

// int main()
// {
//     // fd：文件描述符
//     // mufile：打开的文件名
//     // O_WRONLY ：写方式 | O_CREAT：没有该文件就创建 | O_APPEND : 追加写入
//     int fd = open("myfile", O_WRONLY | O_CREAT | O_APPEND, 0666);
//     if (fd == -1)
//     {
//         perror("open");
//     }

//     int count = 5;
//     char *msge = "hello C++ and Linux\n";

//     while (count--)
//     {
//         ssize_t n = write(fd, msge, strlen(msge));
//         if (n == -1)
//         {
//             perror("write:");
//         }
//     }

//     close(fd);

//     return 0;
// }

// #include <stdio.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>

// int main()
// {
//     // fd：文件描述符
//     // mufile：打开的文件名
//     // ORDONLY:独方式打开
//     int fd = open("myfile", O_RDONLY);
//     if (fd == -1)
//     {
//         perror("open");
//     }

//     char buff[1024];

//     // fd:读取文件的文件描述符
//     // buff:存储读取数据的缓冲区
//     // 1024:最大读取字节数
//     ssize_t n = read(fd, buff, 1024);
//     if (n == -1)
//     {
//         perror("write:");
//     }

//     printf(buff);
//     close(fd);

//     return 0;
// }

// #include <stdio.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>

// int main()
// {
//     // 打开一个文件
//     int fd = open("testfile", O_WRONLY | O_CREAT, 0666);
//     int fd1 = open("testfile1", O_WRONLY | O_CREAT, 0666);
//     int fd2 = open("testfile2", O_WRONLY | O_CREAT, 0666);
//     int fd3 = open("testfile3", O_WRONLY | O_CREAT, 0666);

//     // 打印文件描述符
//     printf("%d\n", fd);
//     printf("%d\n", fd1);
//     printf("%d\n", fd2);
//     printf("%d\n", fd3);

//     return 0;
// }

// #include <stdio.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <string.h>
// int main()
// {
//     char buf[1024];
//     // 0:标准输入的文件描述符——键盘文件
//     ssize_t s = read(0, buf, sizeof(buf));
//     if (s > 0)
//     {
//         buf[s] = 0;
//         // 写入1号文件描述符的文件中——显示器文件
//         // 写入2号文件描述符的文件中——显示器文件
//         write(1, buf, strlen(buf));
//         write(2, buf, strlen(buf));
//     }
//     return 0;
// }

// #include <stdio.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>

// int main()
// {
//     close(0);
//     int fd = open("testfile", O_WRONLY | O_CREAT, 0666);
//     // 如果关闭1号文件描述符
//     close(1);
//     int fd1 = open("testfile1", O_WRONLY | O_CREAT, 0666);
//     int fd2 = open("testfile2", O_WRONLY | O_CREAT, 0666);

//     // 打印文件描述符
//     printf("%d\n", fd);
//     printf("%d\n", fd1);
//     printf("%d\n", fd2);

//     return 0;
// }

// #include <stdio.h>
// #include <unistd.h>
// #include <fcntl.h>
// int main()
// {
//     int fd = open("./log", O_CREAT | O_RDWR, 0666);
//     if (fd < 0)
//     {
//         perror("open");
//         return 1;
//     }
//     close(1);
//     // 将fd对应的文件，重定向到1号文件描述符
//     dup2(fd, 1);
//     for (;;)
//     {
//         char buf[1024] = {0};
//         ssize_t read_size = read(0, buf, sizeof(buf) - 1);
//         if (read_size < 0)
//         {
//             perror("read");
//             break;
//         }
//         printf("%s", buf);
//         fflush(stdout);
//     }
//     return 0;
// }

// #include <stdio.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>

// int main()
// {
//     int fd = open("testfile", O_CREAT | O_WRONLY, 0666);
//     int fd1 = open("testfile1", O_CREAT | O_WRONLY, 0666);

//     printf("%d\n", stdin->_fileno);
//     printf("%d\n", stdout->_fileno);
//     printf("%d\n", stderr->_fileno);
//     printf("%d\n", fd);
//     printf("%d\n", fd1);

//     return 0;
// }

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
    const char *msg0 = "hello printf\n";
    const char *msg2 = "hello write\n";

    printf("%s", msg0);
    write(1, msg2, strlen(msg2));

    fork();
    return 0;
}