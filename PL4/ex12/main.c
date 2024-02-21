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
    sem_unlink("/semClientes"); // controla a entrega dos tickets aos clientes
    sem_unlink("/semSeller");   // controlar quando o seller deve entregar o ticket
    shm_unlink("/shm");
    int i;
    int data_size = sizeof(int), fd;

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
    int * shm = (int *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    sem_t *semClientes, *semSeller;
    if ((semClientes = sem_open("/semClientes", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in semClientes()");
        exit(1);
    }
    if ((semSeller = sem_open("/semSeller", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in semSellers()");
        exit(1);
    }
    pid_t pid = 1;
    *shm=0;
    while (1)
    {
        *shm=*shm+1;
        downS(semSeller); // desativar vendedor
        printf("Vendeu!!!!\n");
        upS(semClientes); // ativar comprador
    }

    sem_unlink("/semClientes"); // controla a entrega dos tickets aos clientes
    sem_unlink("/semSeller");   // controlar quando o seller deve entregar o ticket
    return 0;
}
