#include "Comm.hpp"

int main()
{
    Shm shm(SERVER);
    char *start = shm.getstart();

    while (1)
    {
        char buff[1024] = {0};
        char *str = gets(buff);
        buff[strlen(str)] = 0;
        for (int i = 0; i < strlen(str); i++)
        {
            start[i] = buff[i];
        }
        start[strlen(str)] = 0;
    }
    sleep(20);

    return 0;
}