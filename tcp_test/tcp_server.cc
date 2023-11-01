#include "tcp_server.hpp"

string func(string message)
{
    return message + "(-_-)";
}

int main(int argc, char *argv[])
{
    int port = atoi(argv[1]);
    tcp_server server(port, func);
    server.Init();
    server.Start();

    return 0;
}