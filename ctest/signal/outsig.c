#include <stdio.h>
#include <signal.h>
#include <getopt.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

char *signame[32] = {"SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT", "SIGBUS",
                     "SIGFPE", "SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2", "SIGPIPE", "SIGALRM",
                     "SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT", "SIGSTOP", "SIGTSTP", "SIGTTIN",
                     "SIGTTOU", "SIGURG", "SIGXCPU", "SIGXFSZ", "SIGVTALRM", "SIGPROF", "SIGWINCH",
                     "SIGIO", "SIGPWR", "SIGSYS", "SIGRTMIN"};

int main(int argc, char *argv[])
{
    uint64_t sigblk;
    int i = 0;
    if (optind < argc) {
        sigblk = strtoull(argv[optind++], NULL, 0);
    } else {
        printf("please input a sigblk val\n");
    }

    for (; i < 32; i++)
        if (sigblk & (1 << i)) {
            printf("%-10s is block\n", signame[i]);
        }

    printf("===================================================\n");

    for (i=0; i < 32; i++)
        if (!(sigblk & (1 << i))) {
            printf("%-10s is unblock\n", signame[i]);
        }

}
