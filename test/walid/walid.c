#include <fcntl.h>
#include <stdio.h>


int main()
{
    int fd = open("../paths/walid.txt", O_RDONLY);

    if (fd == -1)
        printf("can not\n");
    else
        printf("can\n");
    return 0;
}