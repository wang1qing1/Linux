#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

enum err
{
    SOCKER_ERR = 1,
    BIND_ERR,
    LISTEN_ERR,
    ACCEPT_ERR,
    CONNECT_ERR
};

int main(int argc, char *argv[])
{
    string port = argv[2];
    string ip = argv[1];

    // 创建TCP套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // 向服务端建立连接
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(port.c_str()));
    // net_aton()将网络主机地址cp从IPv4数字和点符号转换为二进制形式(以网络字节为单位)
    // 并将其存储在inp指向的结构中
    inet_aton(ip.c_str(), &server.sin_addr);
    int count = 1;
    while (connect(sock, (struct sockaddr *)&server, sizeof(server)) != 0)
    {
        sleep(1);
        cout << "正在尝试重连..."
             << "[" << count++ << "/5]" << endl;
        if (count == 6)
            exit(CONNECT_ERR);
    }
    // 连接成功
    while (1)
    {
        string message;
        cout << "Enter>>> ";
        getline(cin, message);
        write(sock, message.c_str(), message.length());

        char buff[1024];
        int n = read(sock, buff, sizeof(buff));
        if (n > 0) // 读取成功
        {
            buff[n] = 0;
            cout << "server:" << buff << endl;
        }
        else if (n == 0) // 服务器端关闭了
        {
            cout << "server quit,me too" << endl;
            break;
        }
        else // 读取出错
        {
            cout << "read err" << endl;
            break;
        }
    }
    close(sock);

    return 0;
}