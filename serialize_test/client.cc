#include <iostream>
#include "Protocol.hpp"
#include "Sock.hpp"

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
        exit(0);
    }
    std::string serverip = argv[1];
    uint16_t serverport = atoi(argv[2]);
    Tcp client;
    client.Connect(serverip, serverport);
    while (1)
    {
        // 构建一个请求
        int x;
        int y;
        char op;
        cout << "Input operand 1: ";
        cin >> x;
        cout << "Input operand 2: ";
        cin >> y;
        cout << "Input operand op: ";
        cin >> op;
        Request request(x, y, op);
        string message = request.serialize();
        send(client.FD(), message.c_str(), message.length(), 0);
        // 接收结果
        char buff[1024];
        recv(client.FD(), buff, sizeof(buff), 0);
        // 构建响应
        string format = buff;
        Responce respon;
        // 反序列化
        respon.deserialize(format);
        cout << "Result :" << respon._result << "Exit code:" << respon._code << endl;
    }

    return 0;
}
