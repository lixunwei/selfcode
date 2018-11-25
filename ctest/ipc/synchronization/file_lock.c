#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/types.h>
#include "sync_common.h"

int main(int argc, char *argv[])
{
    int fd;
    long i,seqno;
    pid_t pid;
    ssize_t n;
    char line[1024];

    pid = getpid();
    fd = open(argv[1], O_RDWR);
    sleep(5);
    file_w_lock(fd);
    for (i = 0; i < 20; i++) {
        sleep(1);
        lseek(fd, 0, SEEK_SET);
        n = read(fd, line, 1024);
        line[n] = '\0';
        n = sscanf(line, "%ld\n", &seqno);
        printf("pid=%ld, seq# = %ld\n", (long)pid, seqno);
        seqno++;

        snprintf(line , sizeof(line), "%ld\n", seqno);
        lseek(fd, 0L, SEEK_SET);
        write(fd, line, strlen(line));
    }
    file_unlock(fd);
}
