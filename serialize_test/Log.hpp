#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <cstdio>
#include <ctime>
#include <cstdarg>
#include <sys/types.h>
#include <unistd.h>

using namespace std;
enum
{
    Debug = 0,
    Info,
    Warning,
    Error,
    Fatal,
    Uknown
};

unordered_map<int, string> Level{{0, "Debug"}, {1, "Info"}, {2, "Warning"}, {3, "Error"}, {4, "Fatal"}, {5, "Uknown"}};

string gettime()
{
    time_t cur = time(nullptr);
    struct tm *t = localtime(&cur);
    char buff[128];
    snprintf(buff, sizeof(buff), "%d-%d-%d %d:%d:%d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    return buff;
}

//[等级+日志时间+进程pid]+[描述信息]。
void Logmessage(int level, const char *format, ...)
{
    char logleft[1024];
    string mess_level = Level[level];
    string mess_time = gettime();
    sprintf(logleft, "[%s][%s][PID:%d]", mess_level.c_str(), mess_time.c_str(), getpid());

    char logright[1024];
    va_list p;
    va_start(p, format);
    vsnprintf(logright, sizeof(logright), format, p);
    va_end(p);
    printf("%s %s\n", logleft, logright);
    // FILE *log = fopen("log.txt", "a");
    // fprintf(log, "%s %s\n", logleft, logright);
    // fclose(log);
}
