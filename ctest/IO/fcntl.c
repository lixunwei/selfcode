#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDWR);

    int val = fcntl(fd, F_GETFL, 0);
    printf("FILE:%o\n", val);
}
