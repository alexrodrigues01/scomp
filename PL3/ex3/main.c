#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE 100000

typedef struct
{
    int number;
    char isepScomp[20];

} isep_struct;

void preencher_array(isep_struct *array)
{
    int i;
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        array[i].number = i;
        strcpy(array[i].isepScomp, "ISEP –SCOMP 2020");
    }
}

void handle_signal() {}

long getCurrentTime()
{
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

int main()
{
    long finish, start;
    int transfer[2], i;

    isep_struct array[ARRAY_SIZE];

    isep_struct array_copy[ARRAY_SIZE];

    if (pipe(transfer) == -1)
    {
        perror("Pipe failed");

        return 1;
    }

    preencher_array(array);

    start = getCurrentTime();

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Fork error");
    }

    if (pid > 0)
    {
        close(transfer[0]);

        for (i = 0; i < ARRAY_SIZE; i++)
        {
            write(transfer[1], &array[i], sizeof(isep_struct));
        }

        close(transfer[1]);
        wait(NULL);
    }
    else
    {
        close(transfer[1]);

        for (i = 0; i < ARRAY_SIZE; i++)
        {
            read(transfer[0], &array_copy[i], sizeof(isep_struct));
        }
        close(transfer[0]);
        exit(0);
    }

    finish = getCurrentTime() - start;

    if(printf("Tempo pipe: %ld\n", finish)<0){
        perror("Printf error");
    }

    start = getCurrentTime();

    isep_struct *array2Copy;

    pid = fork();

    if (pid == -1)
    {
        perror("Fork error");
    }
    int fd2;
    if (pid > 0)
    {
    
        fd2 = shm_open("/ex3", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
        ftruncate(fd2, ARRAY_SIZE * sizeof(isep_struct));
        isep_struct *array2 = (isep_struct *)mmap(NULL, ARRAY_SIZE * sizeof(isep_struct), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
        for (i = 0; i < ARRAY_SIZE; i++)
        {
            array2[i] = array[i];
        }
        sleep(1);
        kill(pid, SIGUSR1);
        wait(NULL);
    }
    else
    {
        struct sigaction act;
        memset(&act, 0, sizeof(struct sigaction));

        sigfillset(&act.sa_mask);
        act.sa_handler = handle_signal;
        act.sa_flags = SA_SIGINFO;
        sigaction(SIGUSR1, &act, NULL);

        pause();

        fd2 = shm_open("/ex3", O_RDWR, S_IRUSR | S_IWUSR);
        ftruncate(fd2, ARRAY_SIZE * sizeof(isep_struct));
        isep_struct *array2Copy = (isep_struct *)mmap(NULL, ARRAY_SIZE * sizeof(isep_struct), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
        int r = shm_unlink("/ex3"); /* removes */
        if (r < 0)
            exit(1);
        exit(0);
    }
    finish = (getCurrentTime() - start)-1000000;

    if(printf("Tempo shared memory: %ld\n", finish)<0){
        perror("Printf error");
    }

    return 0;
}