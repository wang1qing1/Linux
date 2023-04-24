#include <iostream>
#include<cstdio>
#include <cassert>
#include<cstring>
#include<sys/types.h>
#include <cerrno>
#include <sys/stat.h>
#include <fcntl.h>
 #include <unistd.h>


using  namespace std;
#define NUM 1024
const string fifoname = "./fifo";
uint32_t mode =0666;




