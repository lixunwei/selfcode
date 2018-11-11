#include <stdio.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    int flags = O_RDWR | O_CREAT;
    int filemode = S_IRWXU|S_IRWXG|S_IRWXO;
    char *name = NULL;
    mqd_t mid;

    if (optind < argc) {
        name = strdup(argv[optind++]);
    } else {
        printf("please input a mqueue namei!!!\n");
        exit(1);
    }

    if ((mid = mq_open(name, flags, filemode, NULL)) == (mqd_t)-1) {
        printf("Can't create %s mqueue (%s)\n", name, strerror(errno));
        exit(2);
    }

    mq_close(mid);
}
