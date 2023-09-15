#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    printf("***********begin**********\n");
    int sum = 0;
    for (int i = 1; i <= 100; i++)
        sum += i;

    _exit(100);

    printf("***********end**********\n");

    if (sum == 5050)
        return -1;
    else
        return 1;
}