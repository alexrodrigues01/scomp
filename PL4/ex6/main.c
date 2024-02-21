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

int main()
{
    sem_unlink("/sem");
    int i;
    sem_t *sem;
    if ((sem = sem_open("/sem", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in sem()");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Erro no fork");
        return 1;
    }

    for (i = 0; i < 7; i++)
    {
        if (pid == 0)
        {
            printf("I’m the child\n");
            if (sem_post(sem) == -1)
            {
                perror("Erro no decremento do semáforo");
                exit(1);
            }
            if(i==6){
                printf("I’m the child\n");
                exit(0);
            }
        }
        else
        { 
            if (sem_wait(sem) == -1)
            {
                perror("Erro no incremento do semáforo");
                exit(1);
            }
            printf("I’m the father\n");
        }
    }
    sem_unlink("/sem");
    return 0;
}