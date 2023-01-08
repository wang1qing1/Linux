#include"proc.h"

void Proc()
{
  
  char pro[101]={0};
  memset(pro,'\0',sizeof(pro));
  int i=0;
  while(i<=100)
  {
    printf("[\033[40;31m%-100s\033[0m][%d%%]\r",pro,i);
    fflush(stdout);
    pro[i++]='#';
    sleep(1);
  }
  
    printf("\n");
}
