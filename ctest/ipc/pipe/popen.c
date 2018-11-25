#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    FILE *fp = popen("ls -al", "r");
    char buff[1024];

    while(fgets(buff, 1024, fp) != NULL) {
        printf("read: %s\n", buff);
    }
}
