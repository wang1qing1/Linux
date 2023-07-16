#include "udp_client.hpp"

enum
{
    CLIENT_ERR = 0,
    SOCKAT_ERR = 1,
    BIND_ERR,
    CLIENT_SOCKER_ERR
};
static void usage(std::string proc)
{
    std::cout << "Usage:\n\t" << proc << " serverip serverport\n"
              << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        usage(argv[0]);
        exit(CLIENT_ERR);
    }
    // 目的主机IP，和端口
    string serverIP = argv[1];
    uint16_t serverport = atoi(argv[2]);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        cerr << "client create socket error" << endl;
        exit(CLIENT_SOCKER_ERR);
    }
    // client 这里要不要bind呢？要的！socket通信的本质[clientip:clientport, serverip:serverport]
    // 要不要自己bind呢？不需要自己bind，也不要自己bind，OS自动给我们进行bind -- 为什么？client的port要随机让OS分配防止client出现
    // 启动冲突 -- server 为什么要自己bind？1. server的端口不能随意改变，众所周知且不能随意改变的 2. 同一家公司的port号
    // 需要统一规范化

    // 确定server主机
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = inet_addr(serverIP.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);

    while (1)
    {
        string message;
        cout << "please send:";
        getline(cin, message);
        sendto(sock, message.c_str(), message.length(), 0, (struct sockaddr *)&server, sizeof(server));
    }

    return 0;
}