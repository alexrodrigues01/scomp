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
* O pai é o Seller e os filhos os Clientes
*/

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
    int i;
    int data_size = sizeof(int), fd;

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

    sem_t *semClientes, *semSeller;
    if ((semClientes = sem_open("/semClientes", 0)) == SEM_FAILED)
    {
        perror("Error in semClientes()");
        exit(1);
    }
    if ((semSeller = sem_open("/semSeller", 0)) == SEM_FAILED)
    {
        perror("Error in semSellers()");
        exit(1);
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

    downS(semClientes); // espera pela sua vez de pegar o ticket

    srand(getpid());
    sleep(rand() % 11 + 1);

    int *shm = (int *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    printf("O cliente %d tem o ticket %d\n", getpid(), *shm);

    upS(semSeller); // ativar o vendedor
    return 0;
}
