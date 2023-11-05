#pragma once
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
using namespace std;

#define MYSELF 1

#define SEP "\n\r"
#define SEPLEN strlen(SEP)

// str:报文;len:有效载荷的长度
std::string Rehead(std::string str, int len)
{
    return str.substr(str.length() - 2 - len, len);
}

// 报文=报头+有效载荷——————"有效载荷长度"\n\r"有效载荷"\n\r
std::string Addhead(std::string str)
{
    std::string len = to_string(str.length());
    //"7\n\r123+123\n\r"
    return len + SEP + str + SEP;
}

int ReadFormat(int fd, std::string &inputstr, std::string *target)
{
    // 从流中读取—————— "7"+\n\r+"123+321"+\n\r
    char buff[1024];
    int n = recv(fd, buff, sizeof(buff), 0);
    if (n < 0)
    {
        cout << "22222222222222" << endl;
        return n;
    }

    string readstr = buff;
    // cout << readstr << endl;
    // 解析判断读取的字符串是否完整
    // 尝试读取报头
    int pos = readstr.find(SEP, 0);
    if (pos == std::string::npos) // 没有找到分割"\n\r"
        return 0;
    // 找到报头分隔符，提取报头——————有效载荷的长度
    string headstr = readstr.substr(0, pos);
    int len = atoi(headstr.c_str());
    // 计算出整个报文应该有的长度——————,报头+分割符+有效载荷
    int formatlen = headstr.length() + len + 2 * SEPLEN;
    if (readstr.length() < formatlen) // 读取的报文长度小于报文应该有的长度，没有读取完整
        return 0;
    // 读取到一个完整的报文了
    *target = readstr.substr(0, formatlen);
    inputstr.erase(0, formatlen);
    // cout << *target << endl;
    return len;
}

class Request
{
public:
    Request()
    {
    }
    Request(int x, int y, char op)
        : _x(x), _y(y), _op(op)
    {
    }

    // 序列化
    std::string serialize()
    {
#ifdef MYSELF
        string strx = to_string(_x);
        string stry = to_string(_y);
        string strop;
        strop += _op;
#else
        // 使用json
#endif
        string request = strx + strop + stry;
        string requestlen = to_string(request.length());
        string format = requestlen + SEP + request + SEP;
        return format;
    }
    // 反序列化"123+321"
    void deserialize(const std::string &str)
    {
#ifdef MYSELF
        string strx;
        string stry;
        string strop;
        bool isleft = 1;
        for (auto e : str)
        {
            if (e >= '0' && e <= '9' && isleft)
            {
                strx += e;
            }
            else if (e < '0' || e > '9')
            {
                strop += e;
                isleft = 0;
            }
            else if (e >= '0' && e <= '9' && !isleft)
            {
                stry += e;
            }
        }
        _x = atoi(strx.c_str());
        _y = atoi(stry.c_str());
        _op = strop[0];
#else

#endif
    }

public:
    int _x;
    int _y;
    char _op;
};

class Responce
{
public:
    Responce(int result, int code)
        : _result(result), _code(code)
    {
    }

    Responce()
    {
    }
    // 序列化
    std::string serialize()
    {
#ifdef MYSELF
        string strresult = to_string(_result);
        string strcode = to_string(_code);

#else

#endif
        return strresult + SEP + strcode;
    }
    // 反序列化
    void deserialize(const std::string &str)
    {
#ifdef MYSELF
        string strresult;
        string strcode;
        bool isleft = 1;
        for (auto e : str)
        {
            if (e >= '0' && e <= '9' && isleft)
            {
                strresult += e;
            }
            else if (e <= '0' || e >= '9')
            {
                isleft = 0;
            }
            else if (e >= '0' && e <= '9' && !isleft)
            {
                strcode += e;
            }
        }
        _result = atoi(strresult.c_str());
        _code = atoi(strcode.c_str());
#else
        // 使用json
#endif
    }

public:
    int _result;
    int _code;
};