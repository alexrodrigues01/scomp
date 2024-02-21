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

int main()
{
    sem_t *sem;
    if ((sem = sem_open("/sem_ex5", O_CREAT, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Error while forking\n");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        printf("I'm the child\n");
        if (sem_post(sem) == -1)
        {
            perror("Erro no incremento do semáforo");
            exit(1);
        }
        exit(0);
    }
    else
    {
        if (sem_wait(sem) == -1)
        {
            perror("Erro no incremento do semáforo");
            exit(1);
        }
        printf("I'm the father\n");
    }

    sem_unlink("/sem_ex5");

    return 0;
}