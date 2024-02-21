#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>

void handle_SIGUSR1();

void lista_sinais_bloqueados()
{
    int signo;
    sigset_t mask;
    sigprocmask(0, NULL, &mask);
    for (signo = 1; signo <= 64; signo++)
    {
        if (sigismember(&mask, signo) == 1)
            if (printf("Sinal %d bloqueado.\n", signo) < 0)
            {
                perror("Printf Error");
            }
    }
}

void handle_SIGUSR1(int signo)
{
    lista_sinais_bloqueados();
}

int main()
{
    printf("%d", getpid());
    struct sigaction act;

    memset(&act, 0, sizeof(struct sigaction));
    sigfillset(&act.sa_mask);
    act.sa_sigaction = handle_SIGUSR1;
    act.sa_flags = SA_SIGINFO;
    if (sigaction(SIGUSR1, &act, NULL) == -1)
    {
        perror("SigAction Error");
    }

    for (;;)
    {
        if (printf("I'm working!\n") < 0)
        {
            perror("Printf Error");
        }
        sleep(1);
    }

    return 0;
}