#include "myfile.h"

int main()
{

    MYFILE *file = my_fopen("log.txt", "a");

    char *str = "hello c++ and linux\n";

    my_fwrite(str, strlen(str), 1, file);
    my_fwrite(str, strlen(str), 1, file);
    my_fwrite(str, strlen(str), 1, file);
    my_fwrite(str, strlen(str), 1, file);

    my_fclose(file);

    return 0;
}
