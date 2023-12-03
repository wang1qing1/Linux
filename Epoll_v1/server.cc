#include <iostream>
#include "EpollServer.hpp"

int main()
{
    EpollServer epollserver(8081);
    epollserver.Start();

    return 0;
}