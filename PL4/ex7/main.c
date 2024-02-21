#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <signal.h>
#include <string.h>

int main()
{
    int i, j;
    sem_t *sem[3];
    char semaphoreName[100];

    for (j = 0; j < 3; j++)
    {
        sprintf(semaphoreName, "%s%d", "sem_ex7_", j + 1);
        if (j == 0)
        {
            if ((sem[j] = sem_open(semaphoreName, O_CREAT, 0644, 1)) == SEM_FAILED)
            {
                perror("Error in sem_open()");
                exit(1);
            }
        }
        else
        {
            if ((sem[j] = sem_open(semaphoreName, O_CREAT, 0644, 0)) == SEM_FAILED)
            {
                perror("Error in sem_open()");
                exit(1);
            }
        }
    }

    for (i = 0; i < 3; i++)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("Error while forking\n");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            if (i == 0)
            {
                if (sem_wait(sem[0]) == -1)
                {
                    perror("Erro no decremento do semáforo");
                    exit(1);
                }
                printf("\nSistemas ");
                fflush(stdout);
                if (sem_post(sem[1]) == -1)
                {
                    perror("Erro no incremento do semáforo");
                    exit(1);
                }
                if (sem_wait(sem[0]) == -1)
                {
                    perror("Erro no decremento do semáforo");
                    exit(1);
                }
                printf("a ");
                fflush(stdout);
                if (sem_post(sem[1]) == -1)
                {
                    perror("Erro no incremento do semáforo");
                    exit(1);
                }
                exit(0);
            }

            if (i == 1)
            {
                if (sem_wait(sem[1]) == -1)
                {
                    perror("Erro no decremento do semáforo");
                    exit(1);
                }
                printf("de ");
                fflush(stdout);
                if (sem_post(sem[2]) == -1)
                {
                    perror("Erro no incremento do semáforo");
                    exit(1);
                }
                if (sem_wait(sem[1]) == -1)
                {
                    perror("Erro no decremento do semáforo");
                    exit(1);
                }
                printf("melhor ");
                fflush(stdout);

                if (sem_post(sem[2]) == -1)
                {
                    perror("Erro no incremento do semáforo");
                    exit(1);
                }
                exit(0);
            }
            if (i == 2)
            {
                if (sem_wait(sem[2]) == -1)
                {
                    perror("Erro no decremento do semáforo");
                    exit(1);
                }
                printf("Computadores - ");
                fflush(stdout);
                if (sem_post(sem[0]) == -1)
                {
                    perror("Erro no incremento do semáforo");
                    exit(1);
                }
                if (sem_wait(sem[2]) == -1)
                {
                    perror("Erro no decremento do semáforo");
                    exit(1);
                }
                printf("disciplina!\n");
                fflush(stdout);
                exit(0);
            }
        }
    }

    for (i = 0; i < 3; i++)
    {
        wait(NULL);
    }

    sem_unlink("sem_ex7_1");
    sem_unlink("sem_ex7_2");
    sem_unlink("sem_ex7_3");

    return 0;
}