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
#define START_TIME_SESSION 0
#define END_TIME_SESSION 10
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
    sem_unlink("/semLimitePessoas");
    sem_unlink("/semTime");
    int data_size = sizeof(int), fd, i;

    sem_t *semLimitePessoas, *semTime;

    if ((semLimitePessoas = sem_open("/semLimitePessoas", O_CREAT | O_EXCL, 0644, 5)) == SEM_FAILED)
    {
        perror("Error in semLimitePessoas()");
        exit(1);
    }

    if ((semTime = sem_open("/semTime", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in semTime()");
        exit(1);
    }

    pid_t pid = 1;
    for (i = 0; i < 10; i++)
    {
        if (pid > 0)
        {
            pid = fork();
        }
        else
        {
            break;
        }
    }
    if (pid == 0)
    {
        while (1)
        {
            // visitors
            // entrar
            time_t rawtime;
            struct tm *timeinfo;
            time(&rawtime);
            timeinfo = localtime(&rawtime);

            // inicio do show
            if (timeinfo->tm_sec >= START_TIME_SESSION && timeinfo->tm_sec < END_TIME_SESSION)
            {
                downS(semTime); // fica a espera
                upS(semTime); // proxima pessoa pode
            }

            if (i % 2 == 0)
            {
                downS(semLimitePessoas);
                printf("Pessoa entrar\n");
            }
            else
            {

                int value;
                sem_getvalue(semLimitePessoas, &value);
                if (value != 5)
                {
                    //sair
                    printf("Pessoa sair\n");
                    upS(semLimitePessoas);
                }
            }
            sleep(2);
        }
    }
    else
    {
        char annoucement=1;
        while (1)
        {
            time_t rawtime;
            struct tm *timeinfo;
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            if (timeinfo->tm_sec == START_TIME_SESSION && annoucement==1)
            {
                printf("Inicio do Show\n");
                annoucement=0;
            }

            // fim do show
            if (timeinfo->tm_sec == END_TIME_SESSION)
            {
                sleep(1);
                annoucement=0;
                printf("Fim do Show!\n");
                upS(semTime); // proxima pessoa pode
            }
        }

        printf("A duração foi é de 5s\n");

        // sala
    }
    sem_unlink("/semLimitePessoas");
    sem_unlink("/semTime");
    return 0;
}
