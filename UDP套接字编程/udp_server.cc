#include "udp_server.hpp"

int main()
{
    UDP_ser::udpserver udpser(8081);
    udpser.InitServer();
    udpser.Start();

    return 0;
}