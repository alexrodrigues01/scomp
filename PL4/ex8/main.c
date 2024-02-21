#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <signal.h>
#include <string.h>

typedef struct
{
    int diferenca;
} shared_mem;

int main()
{
    int i, fd, data_size = sizeof(shared_mem);
    sem_t *sem1;
    sem_t *sem2;
    shared_mem *mem;

    sem_unlink("/sem_ex8_1");
    sem_unlink("/sem_ex8_2");

    fd = shm_open("/mem_ex8", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("Erro na criação da memória partilhada!\n");
    }
    if (ftruncate(fd, data_size) == -1)
    {
        perror("Erro no ftruncate!\n");
    }
    mem = (shared_mem *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    mem->diferenca = 0;

    if ((sem1 = sem_open("/sem_ex8_1", O_CREAT, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(1);
    }

    if ((sem2 = sem_open("/sem_ex8_2", O_CREAT, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(1);
    }

    for (i = 0; i < 2; i++)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("Error while forking\n");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            if (i == 0)
            {
                while (1)
                {

                    if (sem_wait(sem1) == -1)
                    {
                        perror("Erro no decremento do semáforo");
                        exit(1);
                    }
                    while (mem->diferenca < 1)
                    {
                        printf("S\n");
                        sleep(1);
                        mem->diferenca += 1;
                    }
                    if (sem_post(sem2) == -1)
                    {
                        perror("Erro no incremento do semáforo");
                        exit(1);
                    }
                }
            }
            if (i == 1)
            {
                while (1)
                {
                    if (sem_wait(sem2) == -1)
                    {
                        perror("Erro no decremento do semáforo");
                        exit(1);
                    }
                    while (mem->diferenca > -1)
                    {
                        printf("C\n");
                        sleep(1);
                        mem->diferenca -= 1;
                    }
                    if (sem_post(sem1) == -1)
                    {
                        perror("Erro no incremento do semáforo");
                        exit(1);
                    }
                }
            }
        }
    }
    return 0;
}