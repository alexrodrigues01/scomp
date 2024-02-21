#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

int main()
{
    int fd1[2], i, fd2[2], bet, continueNr = 1;
    pid_t pid;
    time_t t;

    if (pipe(fd1) == -1 || pipe(fd2) == -1)
    {

        perror("Pipe failed");

        return 1;
    }

    pid = fork();

    if(pid==-1){
        perror("Fork error");
    }

    if (pid != 0)
    {
        int saldo = 20, readBet;

        // fechar leitura fd1
        close(fd1[0]);
        // fechar escrita fd2
        close(fd2[1]);
        /* Intializes random number generator */
        srand((unsigned)time(&t));
        while (saldo > 0)
        {
            bet = rand() % 6 + 1;
            if (write(fd1[1], &continueNr, 4) == -1)
            {
                perror("Write Error");
            }

            if (read(fd2[0], &readBet, sizeof(readBet)) == -1)
            {
                perror("Read Error");
            }

            if (printf("NEW BET \nSaldo: %d\n-------\n%d-%d\n-------\n\n", saldo,bet, readBet) < 0)
            {
                perror("Printf Error");
            }

            if (readBet == bet)
            {
                saldo += 10;
            }
            else
            {
                saldo -= 5;
            }
        }
        close(fd2[0]);
        continueNr = 0;
        if (write(fd1[1], &continueNr, 4) == -1)
        {
            perror("Write Error");
        }
    }
    else
    {
        int generate;
        close(fd1[1]);
        close(fd2[0]);

        /* Intializes random number generator */
        srand(getpid());

        do
        {

            if (read(fd1[0], &generate, sizeof(generate)) == -1)
            {
                perror("Read Error");
            }

            if (generate == 1)
            {
                bet = rand() % 6 + 1;

                if (write(fd2[1], &bet, sizeof(bet)) == -1)
                {
                    perror("Write Error");
                }
            }

        } while (generate == 1);
        close(fd2[1]);
        close(fd1[0]);
        exit(0);
    }

    return 0;
}