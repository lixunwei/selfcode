#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "pipe_common.h"

int main(int argc, char *argv[])
{
    pid_t pid;
    int pfd[2];
    
    if (pipe(pfd) < 0) {
        printf("Create pipe failed (%s)\n", strerror(errno));
        exit(CREATE_PIPE_FAILED);
    }
    if ((pid = fork()) == 0) {
        char *readbuf = malloc(1024);
        close(pfd[1]);
        sleep(2);
        read(pfd[0], readbuf, 1024);
        printf("child read [%s]", readbuf);
        exit(0);
    }
    close(pfd[0]);
    if (dup2(pfd[1], STDOUT_FILENO) < 0) {
        printf("copy pipe write client to stdout failed (%s)\n", strerror(errno));
        exit(DUP_FILEDESC_FAILED);
    }
    printf("hello word\n");
}
