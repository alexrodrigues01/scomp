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

// Number of processes
#define NUMBER_PROCESS 8
// Number of numbers to be used in the files
#define NUMBER_NUMBERS 200
// Name of file to read
#define INPUT_FILE "Numbers.txt"
// Name of file to write
#define OUTPUT_FILE "Output.txt"
// Name of semaphores
#define SEMAPHORE "/sem_ex2_"

int main()
{
    int i, j;
    char semaphoreName[100];
    sem_t *sem[NUMBER_PROCESS];
    sprintf(semaphoreName, "%s%d", SEMAPHORE, 1);
    sem[0] = sem_open(semaphoreName, O_CREAT | O_EXCL, 0644, 1);
    if (sem[0] == SEM_FAILED)
    {
        perror("Error while creating the semaphore\n");
        exit(EXIT_FAILURE);
    }

    for (i = 1; i < NUMBER_PROCESS; i++)
    {
        sprintf(semaphoreName, "%s%d", SEMAPHORE, i + 1);
        sem[i] = sem_open(semaphoreName, O_CREAT | O_EXCL, 0644, 1);
        if (sem[i] == SEM_FAILED)
        {
            perror("Error while creating the semaphore\n");
            exit(EXIT_FAILURE);
        }
    }

    FILE *reading;
    FILE *writing;

    for (i = 0; i < NUMBER_PROCESS; i++)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("Error while forking\n");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            if (sem_wait(sem[i]) == -1)
            {
                perror("Erro no decremento do semáforo");
                exit(1);
            }
            reading = fopen("numbers.txt", "r");
            writing = fopen("output.txt", "a");

            int aux = i * NUMBER_NUMBERS;

            for (j = aux; j < (NUMBER_NUMBERS + aux); j++)
            {
                int num = 0;
                fscanf(reading, "%d", &num);
                if (j >= aux && j <= aux + NUMBER_NUMBERS)
                {
                    fprintf(writing, "%d\n", num);
                }
            }

            if (i < NUMBER_PROCESS - 1)
            {
                if (sem_post(sem[i + 1]) == -1)
                {
                    perror("Erro no incremento do semáforo");
                    exit(1);
                }
            }
            fclose(reading);
            fclose(writing);
            exit(0);
        }
    }
    for (i = 0; i < NUMBER_PROCESS; i++)
    {
        wait(NULL);
    }

    int num;
    FILE *output2;
    output2 = fopen("output.txt", "r");
    if (output2 == NULL)
    {
        perror("Error while opening file\n");
        exit(EXIT_FAILURE);
    }
    while (fscanf(output2, "%d\n", &num) != EOF)
    {
        printf("%d\n", num);
    }

    fclose(output2);

    for (i = 0; i < NUMBER_PROCESS; i++)
    {
        sprintf(semaphoreName, "%s%d", SEMAPHORE, i + 1);
        sem_unlink(semaphoreName);
    }

    return 0;
}