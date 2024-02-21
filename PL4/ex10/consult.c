#include "record.h"

int main()
{
    int data_size = size * sizeof(record), fd, fd2, i;
    sem_t *sem;
    record *shm;
    program *shm2;
    if ((sem = sem_open("/sem", 0)) == SEM_FAILED)
    {
        perror("Error in sem()");
        exit(1);
    }

    /*
        Abrir/Criar Array de Records
    */
    fd = shm_open("/shm", O_RDWR, S_IRUSR | S_IWUSR);
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
    shm = (record *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    /*
        Abrir/Criar Array de Funcionalidades
    */

    fd2 = shm_open("/shm2", O_RDWR, S_IRUSR | S_IWUSR);
    if (fd2 == -1)
    {
        printf("Error on attempting to open shared memory!\n");
        exit(EXIT_FAILURE);
    }

    int aux2 = ftruncate(fd2, sizeof(program));
    if (aux2 == -1)
    {
        printf("Error while defining the size of shared memory!\n");
        exit(EXIT_FAILURE);
    }
    shm2 = (program *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);

    int number;
    printf("Number: \n");
    scanf("%d", &number);

    if (sem_wait(sem) == -1)
    {
        perror("Erro no decremento do semáforo");
        exit(1);
    }
    for (i = 0; i < shm2->nrClientes; i++)
    {
        if (number == shm[i].number)
        {
            printf("+==+\n");
            printf("Number: %d\n", shm[i].number);
            printf("Name: %s", shm[i].name);
            printf("Address: %s", shm[i].address);
        }
    }
    if (sem_post(sem) == -1)
    {
        perror("Erro no incremento do semáforo");
        exit(1);
    }
    return 0;
}