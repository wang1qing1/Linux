#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "Server.hpp"
#include "Protocol.hpp"
#include "Log.hpp"
#include "util.hpp"
const string wwwroot = "./html/3";
const string defaupath = "/index.html";

class HttpRequest
{
public:
    HttpRequest()
    {
    }

    ~HttpRequest() {}
    void Print()
    {
        cout << method_ << ":" << url_ << ":" << httpVersion_ << endl;

        for (auto &e : body_)
            cout << e << endl;
        cout << "suffix:" << suffix_ << endl;
        cout << "path:" << path_ << endl;
    }

public:
    std::string method_;            // 请求方法
    std::string url_;               // 资源地址
    std::string httpVersion_;       // 协议版本
    std::vector<std::string> body_; // 报头

    std::string path_;   // 请求资源路径
    std::string suffix_; // 资源类型
};

HttpRequest Deserialize(string &message)
{
    HttpRequest req;
    // 1.拿到请求行
    string oneline = HeadOneLine(message, SEP);
    // 2.解析请求行
    DisposeOneLine(oneline, &req.method_, &req.url_, &req.httpVersion_);
    // 3.解析反序列化报头
    while (!message.empty())
    {
        string mes = HeadOneLine(message, SEP);
        req.body_.push_back(mes);
    }
    // 4.设置请求资源路径 url:/a/b/c
    if (req.url_[req.url_.length() - 1] == '/')
    {
        req.path_ = wwwroot + defaupath; //./html/index.html
    }
    else
    {
        req.path_ = wwwroot + req.url_; //./html/a/b/c
    }

    // 5.设置请求资源类型
    auto pos = req.url_.find('.');
    if (pos == string::npos)
        req.suffix_ = ".html";
    else
        req.suffix_ = req.url_.substr(pos);

    return req;
}

string Dispose(string &message)
{
    // 这里我们一定读取的是一个完整的报文
    // cout << message << endl;
    // 一个网页会有很多的资源，每一个资源(网页,图片,视频),都需要一次http请求来得到
    // 所以我们需要知道，每次请求的资源是什么，即需要知道请求的url是什么,url的类型是什么
    // 反序列化请求
    HttpRequest req = Deserialize(message);
    req.Print();
    // 响应————最简单的一个响应
    // 4.有效载荷
    string body = Readfile(req.path_) + SEP;
    // 1.响应的状态行————"HTTP版本 状态码 状态描述\r\n"
    string request_head = string("HTTP/1.0 200 OK") + SEP;
    // 2.响应报头————Content-Length,Content-Type
    request_head += string("Content-Length: ") + to_string(body.length()) + SEP;
    // cout << "___________________________________________" << endl;
    // cout << req.suffix_ << endl;
    // cout << "___________________________________________" << endl;
    request_head += GetContentType(req.suffix_) + SEP;
    // 3.空行
    request_head += SEP;
    // 整体的响应报文
    string responce = request_head + body;
    return responce;
}

void daemonize()
{
    // 1.忽略SIGPIPE信号
    signal(SIGPIPE, SIG_IGN);
    // 2.更改进程的工作目录
    // chdir();

    // 3.让自己不要成为进程组组长
    if (fork() > 0)
        exit(0);
    // 4.设置自己是一个独立的会话
    setsid();
    // 5.重定向0,1,2
    int fd = 0;
    if (fd = open("dev/null", O_RDWR) != -1)
    {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        // 6.关闭掉不需要的fd
        if (fd > STDERR_FILENO)
        {
            close(fd);
        }
    }
}

int main(int argc, char *argv[])
{
    daemonize();
    uint16_t port = atoi(argv[1]);
    Httpserver httpser(Dispose, port);
    httpser.start();

    return 0;
}
