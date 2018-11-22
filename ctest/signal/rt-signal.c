#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

typedef void sigfunc_rt(int, siginfo_t *, void *);

sigfunc_rt *signal_rt(int signo, sigfunc_rt *func, sigset_t *mask)
{
    struct sigaction act;
    struct sigaction oact;

    act.sa_sigaction = func;
    act.sa_mask = *mask;
    act.sa_flags = SA_SIGINFO;

    if (sigaction(signo, &act, &oact) < 0) {
        return ((sigfunc_rt *) SIG_ERR);
    }
    return (oact.sa_sigaction);
}

static void sig_rt(int signo, siginfo_t *info, void *context)
{
    printf("received signal #%d, code = %d, ival =%d\n", signo, info->si_code, info->si_value.sival_int);
}

int main(int argc, char *argv[])
{
    int i,j;
    pid_t pid;
    sigset_t newset;
    union sigval val;

    if ((pid = fork()) == 0) {
        sigemptyset(&newset);
        sigaddset(&newset, SIGRTMAX);
        sigaddset(&newset, SIGRTMAX - 1);
        sigaddset(&newset, SIGRTMAX - 2);
        sigprocmask(SIG_BLOCK, &newset, NULL);
        signal_rt(SIGRTMAX, sig_rt, &newset);
        signal_rt(SIGRTMAX-1, sig_rt, &newset);
        signal_rt(SIGRTMAX-2, sig_rt, &newset);

        sleep(6);

        sigprocmask(SIG_UNBLOCK, &newset, NULL);

        sleep(3);

        exit(0);
    }

    sleep(3);

    for (i = SIGRTMAX; i >= SIGRTMAX-2; i-- ) {
        for (j=0; j <=2; j++) {
            val.sival_int = j;
            sigqueue(pid, i, val);
            printf("send signal %d, val=%d\n", i, j);
        }
    }
    exit(0);
}
