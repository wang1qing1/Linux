#include "server.hpp"

int main()
{
    cout << "00000000" << endl;

    Udp_server server(8081);

    server.Init();
    server.Start();

    return 0;
}