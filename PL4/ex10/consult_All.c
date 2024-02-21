#include "record.h"

int verificarNumber(int number, record *shm, program *prog);

int main()
{
    /*
    / Criar todas os atributos necessarios
    */
    int data_size = size * sizeof(record), fd, fd2;
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
        printf("Error on attempting to open shared memory1!\n");
        exit(EXIT_FAILURE);
    }
    int aux = ftruncate(fd, data_size);
    if (aux == -1)
    {
        printf("Error while defining the size of shared memory1!\n");
        exit(EXIT_FAILURE);
    }
    shm = (record *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    /*
        Abrir/Criar Array de Funcionalidades
    */

    fd2 = shm_open("/shm2", O_RDWR, S_IRUSR | S_IWUSR);
    if (fd2 == -1)
    {
        printf("Error on attempting to open shared memory2!\n");
        exit(EXIT_FAILURE);
    }

    int aux2 = ftruncate(fd2, sizeof(program));
    if (aux2 == -1)
    {
        printf("Error while defining the size of shared memory!\n");
        exit(EXIT_FAILURE);
    }
    shm2 = (program *)mmap(NULL, sizeof(program), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);

    /**
     *Fim de Criar Atributos 
     */

    int i;
    printf("===+Consulting All +===\n");
    if (sem_wait(sem) == -1)
    {
        perror("Erro no decremento do semáforo");
        exit(1);
    }
    for (i = 0; i < shm2->nrClientes; i++)
    {
        printf("+==+\n");
        fflush(stdout);
        printf("Number: %d\n", shm[i].number);
        fflush(stdout);
        printf("Name: %s", shm[i].name);
        fflush(stdout);
        printf("Address: %s", shm[i].address);
        fflush(stdout);
    }
    if (sem_post(sem) == -1)
    {
        perror("Erro no decremento do semáforo");
        exit(1);
    }
    return 0;
}
