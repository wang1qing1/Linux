#pragma once
#include <iostream>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include "util.hpp"
using namespace std;

#define MYSELF 1

#define SEP "\r\n"
#define SEPLEN strlen(SEP)

int ReadFormat(int fd, std::string &inputstr, std::string *target)
{
}

string responce()
{
}

class Request
{
public:
};

class Responce
{
public:
};