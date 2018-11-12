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
#include <pthread.h>

struct sigevent sigev;

static void notify_thread(union sigval arg)
{
    ssize_t n;
    int buff;
    mqd_t mid = *((mqd_t *)arg.sival_ptr);
    struct mq_attr attr;

    mq_notify(mid, &sigev);
    printf("%s start...\n", __func__);
    mq_getattr(mid, &attr);
    n = mq_receive(mid, (char *)&buff, attr.mq_msgsize, NULL);
    printf("mq_receive length:%d val:%d (%s)\n", n, buff, strerror(errno));
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int flags = O_RDWR | O_CREAT;
    int filemode = S_IRWXU | S_IRWXG | S_IRWXO;
    char *name = NULL;
    struct mq_attr attr;
    mqd_t mid;
    int signo;
    sigset_t zeromask, newmask, oldmask;
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
    sigev.sigev_notify = SIGEV_THREAD;
    sigev.sigev_value.sival_ptr = &mid;
    sigev.sigev_notify_function = notify_thread;
    sigev.sigev_notify_attributes = NULL;
    mq_notify(mid, &sigev);
    printf("enter while \n");
    buf = malloc(attr.mq_msgsize);
    while(1) {
        pause();
    }
}
