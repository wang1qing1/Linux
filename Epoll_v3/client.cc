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
        // 1.构建请求
        Request request(x, y, op);
        // 2.有效载荷序列化
        string message = request.serialize();
        // 3.添加报头
        message = Addhead(message);
        // 4.发送给服务器
        send(tcp.FD(), message.c_str(), message.length(), 0);

        int formatlen = 0;
        string input;
        string target;
        // 1.构建响应
        Responce respon;
        char buff[1024];
        while (1)
        {
            // 2.读取响应
            cout << "读取中......." << endl;
            int n = recv(tcp.FD(), buff, sizeof(buff), 0);
            if (n == 0)
            {
                Logmessage(Debug, "对端关闭");
                break;
            }
            if (n < 0)
            {
                Logmessage(Warning, "errno:%d,%s", errno, strerror(errno));
                break;
            }
            if (n > 0)
            {
                input += buff;
                int formatlen = ReadFormat(input, &target);
                if (formatlen == 0)
                    continue;
                // 读取到一个完整的报文
                cout << "读取到一个完整的报文:" << target << endl;
                // 3.去报头
                string format = Rehead(target, formatlen);
                cout << "报文去报头后:" << format << endl;
                // 4.有效载荷反序列化
                respon.deserialize(format);
                cout << "反序列化:" << respon._result << ":" << respon._result << endl;
                break;
            }
        }
        cout << "Result :" << respon._result << ",Exit code:" << respon._code << endl;
    }
    return 0;
}
