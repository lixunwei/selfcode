#include <stdio.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
    int flags = O_RDWR|O_CREAT;
    int filemode = S_IRWXU | S_IRWXG | S_IRWXO;
    char *name;
    mqd_t mid;
    struct mq_attr attr;
    int buff;

    if (optind < argc) {
        name = strdup(argv[optind++]);
        buff = strtoul(argv[optind++], NULL, 0);
    } else {
        printf("please input a mqueue name!!!\n");
        exit(1);
    }

    if ((mid = mq_open(name, flags, filemode, NULL)) == (mqd_t)-1) {
        printf("Can't open mqueue %s (%s)", name, strerror(errno));
        exit(2);
    }

    mq_getattr(mid, &attr);
    //buff = calloc(128, sizeof(char));
    printf("name : %s send val: %d\n", name, buff);
    mq_send(mid, &buff, sizeof(buff), 0);
}
