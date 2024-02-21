#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wait.h>
#include <time.h>
#include <string.h>

void buy_chips();
void buy_beer();
void eat_and_drink();

int main()
{
    sem_unlink("/sem");
    sem_unlink("/sem2");
    int i;
    sem_t *sem, *sem2;
    if ((sem = sem_open("/sem", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in sem()");
        exit(1);
    }
    if ((sem2 = sem_open("/sem2", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in sem()");
        exit(1);
    }

    int nr = 0;
    pid_t pid = 1;

    for (i = 0; i < 2; i++)
    {
        if (pid > 0)
        {
            pid = fork();
            if (pid == -1)
            {
                perror("Erro no fork");
                return 1;
            }
        }
        else
        {
            break;
        }
    }

    if (pid == 0)
    {
        if (i == 1)
        {
            buy_chips();
            if (sem_post(sem2) == -1)
            {
                perror("Erro no incremento do semáforo");
                exit(1);
            }
            if (sem_wait(sem) == -1)
            {
                perror("Erro no decremento do semáforo");
                exit(1);
            }
        }
        else
        {
            buy_beer();
            if (sem_post(sem) == -1)
            {
                perror("Erro no incremento do semáforo");
                exit(1);
            }
            if (sem_wait(sem2) == -1)
            {
                perror("Erro no decremento do semáforo");
                exit(1);
            }
        }
        eat_and_drink();
        exit(0);
    }
    wait(NULL);
    wait(NULL);
    sem_unlink("/sem2");
    sem_unlink("/sem");
    return 0;
}

void eat_and_drink()
{
    printf("Eating and drinking!\n");
}

void buy_beer()
{
    printf("Buying beer\n");
}
void buy_chips()
{
    printf("Buying chips\n");
}