#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <semaphore.h>

#define BUFFER_SIZE 10
#define NUMBER_PRODUCTS 30
#define NUMBER_PROCESSES 2

typedef struct
{
    int buffer[BUFFER_SIZE];
    char head;
    char tail;
} bufferCircular;

int main(void)
{
    int fd = shm_open("/shm_ex13", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (ftruncate(fd, sizeof(bufferCircular)) == -1)
    {
        perror("Error creating shared memory");
        exit(0);
    }
    bufferCircular *shm = mmap(NULL, sizeof(bufferCircular), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm == NULL)
    {
        perror("Error creating maping");
        exit(0);
    }

    shm->head = 0;
    shm->tail = 0;

    sem_t *sem_write = sem_open("/sem_write", O_CREAT | O_EXCL, 0644, BUFFER_SIZE), *sem_read = sem_open("/sem_read", O_CREAT | O_EXCL, 0644, 0), *sem_producer = sem_open("/sem_producer", O_CREAT | O_EXCL, 0644, 1);
    if (sem_read == NULL || sem_producer == NULL || sem_write == NULL)
    {
        perror("Error in sem_open()");
        exit(1);
    }

    //Produtor
    int i;
    for (i = 0; i < NUMBER_PROCESSES; i++)
    {
        pid_t p = fork();
        if (p == -1)
        {
            perror("Error while forking");
            exit(0);
        }
        if (p == 0)
        {
            int j, products = NUMBER_PRODUCTS / NUMBER_PROCESSES;
            for (j = i * products; j < products * (i + 1); j++)
            {
                if (sem_wait(sem_producer) == -1 || sem_wait(sem_write) == -1)
                {
                    perror("Erro no decremento do semáforo");
                    exit(1);
                }

                shm->buffer[shm->head] = j;
                shm->head = (shm->head + 1) % BUFFER_SIZE;

                if (sem_post(sem_read) == -1 || sem_post(sem_producer) == -1)
                {
                    perror("Erro no incremento do semáforo");
                    exit(1);
                }
            }

            if (munmap(shm, sizeof(bufferCircular)) == -1)
                perror("Error while munmapping");
            if (close(fd) == -1)
                perror("Error while Closing (shared memory)");
            exit(0);
        }
    }

    //Consumidor
    for (i = 0; i < NUMBER_PRODUCTS; i++)
    {
        if (sem_wait(sem_read) == -1)
        {
            perror("Erro no decremento do semáforo");
            exit(1);
        }

        printf("Consumidor Recebeu: %d\n", shm->buffer[shm->tail]);
        shm->tail = (shm->tail + 1) % BUFFER_SIZE;

        if (sem_post(sem_write) == -1)
        {
            perror("Erro no incremento do semáforo");
            exit(1);
        }
    }

    for (i = 0; i < NUMBER_PROCESSES; i++)
        if (wait(NULL) == -1)
            perror("Error waiting");

    if (sem_unlink("/sem_write") == -1 || sem_unlink("/sem_read") == -1 || sem_unlink("/sem_producer") == -1)
        perror("Error while unlinking sem");

    if (munmap(shm, sizeof(bufferCircular)) == -1)
        perror("Error while munmapping");
    if (shm_unlink("/shm_ex13") == -1)
        perror("Error shm_unlink Parent");
    return 0;
}
