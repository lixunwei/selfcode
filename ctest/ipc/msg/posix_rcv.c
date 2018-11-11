#include <stdio.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


int main(int argc, char *argv[])
{
    int flags = O_RDWR | O_CREAT;
    int filemode = S_IRWXU | S_IRWXG | S_IRWXO;
    char *name = NULL;
    struct mq_attr attr;
    mqd_t mid;
    int signo;
    sigset_t zeromask, newmask, oldmask;
    struct sigevent sigev;
    char *buf;
    int length;
    int int_val;

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
    mq_getattr(mid, &attr);
    //buf = malloc(attr.mq_msgsize);
    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigemptyset(&oldmask);

    sigaddset(&newmask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);

    sigev.sigev_notify = SIGEV_SIGNAL;
    sigev.sigev_signo = SIGUSR1;
    mq_notify(mid, &sigev);
    printf("enter while \n");
    buf = malloc(attr.mq_msgsize);
    while(1) {
        sigwait(&newmask, &signo);
        printf("signal coming... \n");
        mq_notify(mid, &sigev);
        length = mq_receive(mid, (char *)&int_val, attr.mq_msgsize, NULL);
        printf("message (%s)\n", strerror(errno));
        printf("get a %d val %d\n", length, int_val);
    }
}
