#include<iostream>
#include<string>

using namespace std;


#define PATHNAME "."
#define PROJID 0x6666

const int gsize=4096;

key_t getkey()
{
    key_t k = ftok(PATHNAME,PROJID);
    if(k==-1)
    {
        cerr<<"error: "<<errno<<":"<<strerror(errno)<<endl;
        exit(1);
    }
    return k;
}

