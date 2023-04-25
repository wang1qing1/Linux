#include "comm.hpp"
int main()
{
    Shm shm = Shm(CLIENT);
    char*cli=shm.getstart();
    while(1)
    {
        cout<<"client: ";
        cin>>cli;
        if(strcmp(cli,"quit")==0)
        {
            break;
        }
    }
    return 0;
}