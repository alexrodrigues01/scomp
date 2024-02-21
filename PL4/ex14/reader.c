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

/*
*
*/

typedef struct
{
    char linha[100];
    int writers;
    int readers;
} sharedMemory;

void upS(sem_t *sem)
{
    if (sem_post(sem) == -1)
    {
        perror("Erro no incremento sem;");
        exit(1);
    }
}

void downS(sem_t *sem)
{
    if (sem_wait(sem) == -1)
    {
        perror("Erro no decremento sem;");
        exit(1);
    }
}

int main()
{

    int data_size = sizeof(sharedMemory), fd;

    fd = shm_open("/shm", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        fd = shm_open("/shm", O_RDWR, S_IRUSR | S_IWUSR);
        if (fd == -1)
        {
            printf("Error on attempting to open shared memory!\n");
            exit(EXIT_FAILURE);
        }
    }

    int aux = ftruncate(fd, data_size);
    if (aux == -1)
    {
        printf("Error while defining the size of shared memory!\n");
        exit(EXIT_FAILURE);
    }

    sharedMemory *shm = (sharedMemory *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sem_t *semReader, *reader;

    if ((semReader = sem_open("/semReader", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        if ((semReader = sem_open("/semReader", 0)) == SEM_FAILED)
        {
            perror("Error in semReader()");
            exit(1);
        }
    }

    if ((reader = sem_open("/read", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        if ((reader = sem_open("/read", 0)) == SEM_FAILED)
        {
            perror("Error in read()");
            exit(1);
        }
    }

    if (fork() == -1)
    {
        perror("Error while forking\n");
        exit(EXIT_FAILURE);
    }
    if (fork() == -1)
    {
        perror("Error while forking\n");
        exit(EXIT_FAILURE);
    }
    if (fork() == -1)
    {
        perror("Error while forking\n");
        exit(EXIT_FAILURE);
    }

    // aumentar nr de readers

    downS(semReader);

        shm->readers++;

    upS(semReader);

        // read

        downS(reader);

            printf("\nFrase>\n%sNr Readers: %d\n", shm->linha, shm->readers);
    shm->readers--;
    upS(reader);
    if (shm->readers == 0)
    {
        sem_unlink("/semReader");
        sem_unlink("/read");
        sem_unlink("/write");
        sem_unlink("/semWriter");
        shm_unlink("/shm");
    }

    return 0;
}
