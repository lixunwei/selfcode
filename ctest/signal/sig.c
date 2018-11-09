#include <stdio.h>
#include <signal.h>

void int_handler(int signo)
{
    printf("catch sig int signal\n");
}

int main(int argc, char *argv[])
{
    sigset_t newset;
    sigset_t oldset;
    struct sigaction sig_act;

    sigfillset(&newset);
    //sigdelset(&newset, SIGINT);

    sigprocmask(SIG_BLOCK, &newset, &oldset);

    sig_act.sa_handler = int_handler;
    sigaction(SIGINT, &sig_act, NULL);

    sigsuspend(&newset);

    printf("end\n");


    return;
}
