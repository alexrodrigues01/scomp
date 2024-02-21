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
    sem_unlink("/semPorta1");
    sem_unlink("/semPorta2");
    sem_unlink("/semPorta3");
    sem_unlink("/semLugaresVazios");

    int i;
    sem_t *semPorta1, *semPorta2, *semPorta3, *semLugaresVazios;

    if ((semPorta1 = sem_open("/semPorta1", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem()");
        exit(1);
    }

    if ((semPorta2 = sem_open("/semPorta2", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem()");
        exit(1);
    }

    if ((semPorta3 = sem_open("/semPorta3", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem()");
        exit(1);
    }

    if ((semLugaresVazios = sem_open("/semLugaresVazios", O_CREAT | O_EXCL, 0644, 200)) == SEM_FAILED)
    {
        perror("Error in sem()");
        exit(1);
    }
    i = 0;
    while (1)
    {
        int exp;
        sem_getvalue(semLugaresVazios, &exp);
        printf("\n%d\n", exp);
        i++;

        pid_t pid = 1;
        int a;
        for (a = 0; a < 2; a++)
        {
            pid = fork();
            if (pid == -1)
            {
                perror("Erro no fork");
                return 1;
            }
            if (pid == 0)
            {
                break;
            }
        }

        // criar ação
        if (pid == 0)
        {
            int value1;
            if (a == 1)
            {
                sleep(1);
                // PRIMEIRA PORTA
                downS(semLugaresVazios);
                downS(semPorta1);
                // enter 1 pessoa
                upS(semPorta1);
                printf("%d,Entrar pessoa porta 1\n", getpid());
                sem_getvalue(semLugaresVazios, &value1);
                printf("FILHO=%d,Numero de Pessoas Neste momento: %d\n", getpid(), 200 - value1);
                sleep(1);
                // SEGUDNA PORTA
                downS(semLugaresVazios);
                downS(semPorta2);
                // enter 1 pessoa
                upS(semPorta2);
                printf("%d,Entrar pessoa porta 2\n", getpid());
                sem_getvalue(semLugaresVazios, &value1);
                printf("FILHO=%d,Numero de Pessoas Neste momento: %d\n", getpid(), 200 - value1);
                sleep(1);
                // TERCEIRA PORTA
                downS(semLugaresVazios);
                downS(semPorta3);
                // enter 1 pessoas
                upS(semPorta3);
                printf("\n%d,Entrar pessoa porta 3\n", getpid());
                sem_getvalue(semLugaresVazios, &value1);
                printf("FILHO=%d,Numero de Pessoas Neste momento: %d\n", getpid(), 200 - value1);
            }
            else
            {

                // sair
                int value;
                sem_getvalue(semLugaresVazios, &value);
                if (value == 200)
                {
                    sleep(1);
                }
                // primeira porta
                if (value != 200)
                {
                    sleep(1);
                    upS(semLugaresVazios);
                    downS(semPorta1);
                    // enter 1 pessoa

                    upS(semPorta1);
                    printf("%d,Numero de Pessoas Neste momento: %d\n", getpid(), 200 - value);
                    printf("%d,Sair pessoa porta 1\n", getpid());
                }

                sem_getvalue(semLugaresVazios, &value);
                // segunda porta
                if (value != 200)
                {
                    sleep(1);
                    upS(semLugaresVazios);
                    downS(semPorta2);
                    // enter 1 pessoa

                    upS(semPorta2);
                    printf("%d,Numero de Pessoas Neste momento: %d\n", getpid(), 200 - value);
                    printf("%d,Sair pessoa porta 2\n", getpid());
                }

                sem_getvalue(semLugaresVazios, &value);
                // terceira porta
                if (value != 200)
                {
                    sleep(1);
                    upS(semLugaresVazios);
                    downS(semPorta3);
                    // enter 1 pessoa

                    upS(semPorta3);
                    printf("%d,Numero de Pessoas Neste momento: %d\n", getpid(), 200 - value);
                    printf("%d,Sair pessoa porta 3\n", getpid());
                }
            }
            exit(0);
        }
        wait(NULL);
    }
    sem_unlink("/semPorta1");
    sem_unlink("/semPorta2");
    sem_unlink("/semPorta3");
    sem_unlink("/semLugaresVazios");
    return 0;
}
