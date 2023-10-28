#include "server.hpp"

int main()
{
    Udp_server server(8081);
    server.Init();
    server.Start();
    return 0;
}