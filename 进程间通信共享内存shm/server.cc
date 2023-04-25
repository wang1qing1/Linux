#include "comm.hpp"

int main()
{
    Shm shm=Shm(SERVER);
    char *ser=shm.getstart();
    while(1)
    {
        if(strcmp(ser,"quit")==0)
        {
            break;
        }
        cout<<"server: "<<ser<<endl;
        sleep(1);
    }
    sleep(1);

    return 0;
}