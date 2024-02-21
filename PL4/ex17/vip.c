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

typedef struct
{
    int totalLugaresOcupados;
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

    int fd, data_size = sizeof(info);

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

    // fim da criacao memoria partilhada

    sem_t *semVip, *semEM, *semTeatro, *semLibertaLugar;

    if ((semVip = sem_open("/semVip", 0)) == SEM_FAILED)
    {
        perror("Error in semVip()");
        exit(1);
    }
    if ((semEM = sem_open("/semEM", 0)) == SEM_FAILED)
    {
        perror("Error in semEM()");
        exit(1);
    }
    if ((semTeatro = sem_open("/semTeatro", 0)) == SEM_FAILED)
    {
        perror("Error in semTeatro()");
        exit(1);
    }
    if ((semLibertaLugar = sem_open("/semLibertaLugar", 0)) == SEM_FAILED)
    {
        perror("Error in semLibertaLugar()");
        exit(1);
    }
    // Chegar à fila
    upS(semTeatro);

    // esperar na fila
    downS(semVip);

    downS(semEM);
    shm->totalLugaresOcupados++;
    upS(semEM);

    int value;
    sem_getvalue(semEM, &value);

    // Depois de entrar
    printf("VIP - Entrou\n");

    sleep(10);
    sem_getvalue(semEM, &value);

    downS(semEM);

    shm->totalLugaresOcupados--;

    upS(semEM);

    upS(semLibertaLugar);

    printf("VIP - Saiu\n");

    return 0;
}
