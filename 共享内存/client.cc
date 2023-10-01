#include "Comm.hpp"

int main()
{
    Shm shm(CLIENT);
    char *start = shm.getstart();
    while (1)
    {
        sleep(1);
        printf("%s\n", start);
    }

    sleep(15);

    return 0;
}