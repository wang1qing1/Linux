#include <string.h>
#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

const char *namepipe = "./namepipe";

mode_t mode = 0666;
