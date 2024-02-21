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
#define RANDOM 100
#define CHILD_LIMIT 200
#define CHILDS 8
#define NUMBERS 1600

int main()
{
    int i, j;
    sem_t *sem = sem_open("/sem_ex1", O_CREAT | O_EXCL, 0644, 1);
    if (sem == SEM_FAILED)
    {
        printf("Error while creating the semaphore\n");
        exit(EXIT_FAILURE);
    }

    srand((unsigned)time(NULL));
    FILE *inputFile;
    inputFile = fopen("numbers.txt", "w");
    for (i = 0; i < NUMBERS; i++)
    {
        fprintf(inputFile, "%d ", rand() % RANDOM);
    }
    fclose(inputFile);

    FILE *reading;
    FILE *writing;

    for (i = 0; i < CHILDS; i++)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            printf("Error while forking\n");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            if (sem_wait(sem) == -1)
            {
                perror("Erro no decremento do semáforo");
                exit(1);
            }
            reading = fopen("numbers.txt", "r");
            writing = fopen("output.txt", "a");

            int aux = i * CHILD_LIMIT;

            for (j = aux; j < (CHILD_LIMIT + aux); j++)
            {
                int num = 0;
                fscanf(reading, "%d", &num);
                if (j >= aux && j <= aux + CHILD_LIMIT)
                {
                    fprintf(writing, "%d\n", num);
                }
            }
            fclose(reading);
            fclose(writing);

            if (sem_post(sem) == -1)
            {
                perror("Erro no incremento do semáforo");
                exit(1);
            }
            exit(0);
        }
    }
    for (i = 0; i < CHILDS; i++)
    {
        wait(NULL);
    }

    int num;
    FILE *output2;
    output2 = fopen("output.txt", "r");
    if (output2 == NULL)
    {
        printf("Error while opening file\n");
        exit(EXIT_FAILURE);
    }
    while (fscanf(output2, "%d\n", &num) != EOF)
    {
        printf("%d\n", num);
    }

    fclose(output2);
    sem_unlink("/sem_ex1");

    return 0;
}