#include "clinent.hpp"

//./client ip port
void *Recv(void *argc)
{
    int *fd = static_cast<int *>(argc);
    while (1)
    {
        // 接受信息
        char buff[1024];
        struct sockaddr_in sersock;
        socklen_t sersock_len = sizeof(sersock);
        int n = recvfrom(*fd, buff, sizeof(buff), 0, (struct sockaddr *)&sersock, &sersock_len);
        if (n > 0)
            buff[n] = 0;
        else
            continue;
        cout << "群消息:" << buff << endl;
    }
}

int main(int argc, char *args[])
{

    // 1.创建按套接字
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    // 2.获取ip和端口
    string server_ip = args[1];
    int server_prot = atoi(args[2]);
    // 3.明确连接的服务器
    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_ip.c_str());
    server.sin_port = htons(server_prot);
    // 创建线程
    Thread t(1, Recv, &fd);
    t.run();

    while (1)
    {
        string str;
        str.clear();
        getline(cin, str);

        // 发送信息
        sendto(fd, str.c_str(), str.length(), 0, (struct sockaddr *)&server, sizeof(server));
    }
    t.join();

    return 0;
}