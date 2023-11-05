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
    Tcp tcp;
    tcp.Connect(serverip, serverport);
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
        send(tcp.FD(), message.c_str(), message.length(), 0);
        int formatlen = 0;
        string input;
        string target;
        Responce respon;
        while (1)
        {
            int formatlen = ReadFormat(tcp.FD(), input, &target);
            if (formatlen == 0)
                continue;
            if (formatlen < 0)
                break;
            if (formatlen > 0)
            {
                // 读取到一个完整的报文
                cout << "读取到一个完整的报文:" << target << endl;
                // 去报头
                string format = Rehead(target, formatlen);
                cout << "报文去报头后:" << format << endl;
                // 构建响应,反序列化
                respon.deserialize(format);
                cout << "反序列化:" << respon._result << ":" << respon._result << endl;
                break;
            }
        }
        cout << "Result :" << respon._result << ",Exit code:" << respon._code << endl;
    }
    return 0;
}
