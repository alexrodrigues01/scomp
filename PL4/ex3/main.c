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

typedef struct
{
    char line[80];
} string;

int main()
{
    string *shm;
    int data_size = sizeof(shm) * 100, fd, i;
    pid_t pid = 1;
    sem_t *sem;
    fd = shm_open("/shm", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        printf("Error on attempting to open shared memory!\n");
        exit(EXIT_FAILURE);
    }
    int aux = ftruncate(fd, data_size);
    if (aux == -1)
    {
        printf("Error while defining the size of shared memory!\n");
        exit(EXIT_FAILURE);
    }
    if ((sem = sem_open("/sem", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem()");
        exit(1);
    }
    for (i = 0; i < 5; i++)
    {
        if (pid > 0)
        {
            pid = fork();
        }
        else
        {
            break;
        }
    }

    if (sem_wait(sem) == -1)
    {
        perror("Erro no decremento do semáforo");
        exit(1);
    }
    shm = (string *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    printf("I’m the Father - with PID %d\n", getpid());
    sprintf(shm[i].line, "I'm the Father - with PID %d\n", getpid());
    sleep(2);
    if (sem_post(sem) == -1)
    {
        perror("Erro no incremento do semáforo");
        exit(1);
    }

    if (pid == -1)
    {
        perror("Error while forking\n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        exit(0);
    }
    sem_unlink("/sem");
    if (munmap(shm, data_size) < 0)
    {
        printf("Error  munmap()!\n");
        exit(EXIT_FAILURE);
    }
    // Close fd
    if (close(fd) < 0)
    {
        printf("Error  close()!\n");
        exit(EXIT_FAILURE);
    }

    shm_unlink("/shm");

    return 0;
}