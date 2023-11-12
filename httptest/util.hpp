#pragma once
#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "Protocol.hpp"
#define SEP "\r\n"
using namespace std;

string Readfile(const string path)
{
    // 1. 获取文件本身的大小
    string fileContent;
    struct stat st;
    int n = stat(path.c_str(), &st);
    if (n < 0)
        return "";
    int size = st.st_size;
    // 2. 调整string的空间
    fileContent.resize(size);
    // 3. 读取
    int fd = open(path.c_str(), O_RDONLY);
    if (fd < 0)
        return "";
    read(fd, (char *)fileContent.c_str(), size);
    close(fd);
    return fileContent;
}

string HeadOneLine(string &message, const string &sep)
{
    auto pos = message.find(sep, 0);
    if (pos == string::npos)
        return "";
    string oneline = message.substr(0, pos);
    message.erase(0, pos + sep.size());
    return oneline;
}

void DisposeOneLine(const string &oneline, string *method, string *url, string *httpVersion)
{
    stringstream line(oneline);
    line >> *method >> *url >> *httpVersion;
}

string GetContentType(const string &suffix)
{
    std::string content_type = "Content-Type: ";
    if (suffix == ".html" || suffix == ".htm")
        content_type + "text/html";
    else if (suffix == ".css")
        content_type += "text/css";
    else if (suffix == ".js")
        content_type += "application/x-javascript";
    else if (suffix == ".png")
        content_type += "image/png";
    else if (suffix == ".jpg")
        content_type += "image/jpeg";
    else
    {
    }
    return content_type;
}
