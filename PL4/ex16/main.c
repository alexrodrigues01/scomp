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

#define NR_PROCESSES 10

/*
*
*/

typedef struct
{
    char nrCarrosWest;
    char nrCarrosEast;
    char nrCarrosEspera;
} info;

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
    sem_unlink("/semWest");
    sem_unlink("/semEast");
    sem_unlink("/semGlobal");
    shm_unlink("/shm");

    int fd, i, data_size = sizeof(info);

    // criar memoria partilhada
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

    info *shm = (info *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    shm->nrCarrosEast = 0;
    shm->nrCarrosWest = 0;
    shm->nrCarrosEspera = 0;
    // fim da criacao memoria partilhada

    sem_t *semWest, *semEast, *semGlobal;
    if ((semWest = sem_open("/semWest", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in semWest()");
        exit(1);
    }
    if ((semEast = sem_open("/semEast", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in semEast()");
        exit(1);
    }
    if ((semGlobal = sem_open("/semGlobal", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in semGlobal()");
        exit(1);
    }

    pid_t pid = 1;
    for (i = 0; i < NR_PROCESSES; i++)
    {
        if (pid > 0)
        {
            pid = fork();
            if (pid == -1)
            {
                perror("Fork Error!\n");
            }
        }
        else
        {
            break;
        }
    }

    int value;

    if (pid == 0 && i % 2 == 0)
    {
        // West
        if (shm->nrCarrosEast > 0)
        {
            shm->nrCarrosEspera++;
            // fica a espera
            downS(semWest);
        }
        else
        {
            if (shm->nrCarrosWest == 0)
            {
                downS(semGlobal);
            }
        }

        printf("Carro a ir Para West\n");
        // aumento o nr de carros
        shm->nrCarrosWest++;

        // Assume that a car takes 30 seconds to cross the bridge
        
        sleep(2);

        // diminuir o nr de carros
        shm->nrCarrosWest--;

        // se foi o ultimo carro
        if (shm->nrCarrosWest == 0)
        {

            printf("Ultimo Carro West\n");
            while (shm->nrCarrosEspera > 0)
            {
                upS(semEast);
                shm->nrCarrosEspera--;
            }
            upS(semGlobal);
        }
        exit(0);
    }
    else if (pid == 0 && i % 2 != 0)
    {
        // East
        if (shm->nrCarrosWest > 0)
        {
            // fica a espera
            shm->nrCarrosEspera++;
            downS(semEast);
        }
        else
        {
            if (shm->nrCarrosEast == 0)
            {
                downS(semGlobal);
            }
        }

        printf("Carro a ir Para East\n");
        // aumento o nr de carros
        shm->nrCarrosEast++;
        // Assume that a car takes 30 seconds to cross the bridge
        sleep(2);

        // diminuir o nr de carros
        shm->nrCarrosEast--;

        // se foi o ultimo carro
        if (shm->nrCarrosEast == 0)
        {
            printf("Ultimo Carro East\n");
            while (shm->nrCarrosEspera > 0)
            {
                upS(semWest);
                shm->nrCarrosEspera--;
            }
            upS(semGlobal);
        }
        exit(0);
    }

    for (i = 0; i < NR_PROCESSES; i++)
    {
        wait(NULL);
    }

    sem_unlink("/semWest");
    sem_unlink("/semEast");
    shm_unlink("/shm");
    sem_unlink("/semGlobal");
    return 0;
}
