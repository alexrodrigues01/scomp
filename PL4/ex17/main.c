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
    sem_unlink("/semVip");
    sem_unlink("/semNormal");
    sem_unlink("/semEspecial");
    sem_unlink("/semEM");
    sem_unlink("/semTeatro");
    sem_unlink("/semLibertaLugar");
    shm_unlink("/shm");

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

    sem_t *semVip, *semNormal, *semEspecial, *semEM, *semTeatro,*semLibertaLugar;

    if ((semVip = sem_open("/semVip", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in semVip()");
        exit(1);
    }
    if ((semNormal = sem_open("/semNormal", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in semNormal()");
        exit(1);
    }
    if ((semEspecial = sem_open("/semEspecial", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in semEspecial()");
        exit(1);
    }
    if ((semEM = sem_open("/semEM", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in semEM()");
        exit(1);
    }

    if ((semTeatro = sem_open("/semTeatro", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in semTeatro()");
        exit(1);
    }
    
    // controlar se o teatro está cheio 
    if ((semLibertaLugar = sem_open("/semLibertaLugar", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in semLibertaLugar()");
        exit(1);
    }

    // Ocupar Lugares
    shm->totalLugaresOcupados = 300;

    // saiu 1 pessoa
    sleep(1);
    shm->totalLugaresOcupados = 298;

    while (1)
    {
        // espera por alguem na fila
        printf("Teatro\n");
        downS(semTeatro);

        // se estiver cheio
        downS(semEM);
        int value = shm->totalLugaresOcupados;
        upS(semEM);

        if (value == 300)
        {
            printf("Está cheio !!!\n") ;
            // fica a espera que alguém saia (assumindo que quando sai dá upS(semTeatro) )
            downS(semLibertaLugar);
        }

        // saiu pessoa

        int i = 0, lugares;

        downS(semEM);
        lugares = shm->totalLugaresOcupados;
        upS(semEM);

        int totalAnterior = lugares;
        while (totalAnterior + 1 != lugares)
        {
            if (i == 0)
            {
                // vip
                upS(semVip);
            }
            else if (i == 1)
            {
                // Especial
                upS(semEspecial);
            }
            else if (i == 2)
            {
                // Normal
                upS(semNormal);
            }
            else
            {
                // Ninguem na fila
                break;
            }

            // deixar entrar
            sleep(1);

            downS(semEM);
            lugares = shm->totalLugaresOcupados;
            upS(semEM);

            // Este caso acontece quando damos Up e aumentamos o index do Semaforo , o que não é suposto pois não entrou ninguem
            if (lugares == totalAnterior)
            {
                if (i == 0)
                {
                    // vip
                    downS(semVip);
                }
                else if (i == 1)
                {
                    // Especial
                    downS(semEspecial);
                }
                else if (i == 2)
                {
                    // Normal
                    downS(semNormal);
                }
            }

            i++;
        }
    }

    return 0;
}
