#include <stdio.h>
#include <string.h>
#include <unistd.h>
int main()
{
  char buf[100];
  char tmp[4] = {'/', '-', '*', '|'};
  memset(buf, 0, sizeof(buf));
  for (int i = 0; i < 100; i++)
  {
    buf[i] = '#';
    printf("[%-100s][%d%%][%c]\r", buf, i, tmp[i % 4]);
    if (i == 99)
    {
      printf("\n");
    }
    fflush(stdout);
    usleep(100000);
  }

  return 0;
}
