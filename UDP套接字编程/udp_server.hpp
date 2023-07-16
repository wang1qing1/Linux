#include <iostream>
#include <cstdio>
#include <cerrno>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;
const static uint16_t Port = 8080;

namespace UDP_ser
{
    enum
    {
        SOCKAT_ERR = 1,
        BIND_ERR
    };
    class udpserver
    {
    public:
        udpserver(uint16_t port = Port)
            : _port(port)
        {
            cout << "udp_server port:" << _port << endl;
        }

        void InitServer()
        {
            // 1.创建socket接口，打开网络文件
            //  参数1：选择互联网协议
            //  参数2：具体得通信协议UDP协议
            //  参数3:
            //  返回值：是一个用于网络通信的文件的文件描述符，如果创建失败返回-1，
            //  错误码被设置。
            _fsock = socket(AF_INET, SOCK_DGRAM, 0);
            if (_fsock < 0)
            {
                cerr << "create socket error" << strerror(errno) << endl;
                exit(SOCKAT_ERR);
            }
            cout << "socket success"
                 << "file  descriptor:" << _fsock << endl;
            // 2.给服务器指明IP地址和端口号(port)
            struct sockaddr_in local;
            local.sin_family = AF_INET;

            // local.sin_addr = _ip;C++支持
            // htons()将存储字节序，转换成网络字节序。
            local.sin_port = htons(_port);
            // inet_addr()
            // 功能1：将ip字符产转换成整数
            // 功能2：将主机序列转换成网络序列

            // INADDR_ANY,云服务器，服务器，一般不要指明一个确定的IP
            // 启动udp_server时可以获取服务器的任意一个ip
            local.sin_addr.s_addr = INADDR_ANY;
            if (bind(_fsock, (struct sockaddr *)&local, sizeof(local)) < 0)
            {
                cerr << "bind socket error" << strerror(errno) << endl;
                exit(BIND_ERR);
            }
            cout << "bind socket success: " << _fsock << endl;
        }
        void Start()
        {
            char buffer[1024];
            while (1)
            {
                struct sockaddr_in peer; // 接收客户端的信息(IP和端口)
                socklen_t len = sizeof(peer);
                // 接受客户端的信息
                int n = recvfrom(_fsock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&peer, &len);
                if (n > 0)
                {
                    buffer[n] = 0;
                }
                else
                {
                    continue;
                }

                // 提取客户端信息
                string clientIP = inet_ntoa(peer.sin_addr);
                uint16_t client_port = ntohs(peer.sin_port);
                cout << "ip:" << clientIP << "port" << client_port << "-->" << buffer << endl;
            }
        }
        ~udpserver()
        {
            close(_fsock);
        }

    private:
        int _fsock;     // 文件描述符
        uint16_t _port; // 端口号
        string _ip;     // ip地址
    };
}
