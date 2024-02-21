#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#define NR_PIPES 6

int main()
{
    int fd[NR_PIPES][2], i, nrGenerated, nrToCompare;
    pid_t pid;

    for (i = 0; i < NR_PIPES; i++)
    {

        if (pipe(fd[i]) == -1)
        {

            perror("Pipe failed");

            return 1;
        }
    }

    for (i = 0; i < NR_PIPES - 1; i++)
    {
        pid = fork();

        if(pid==-1){
            perror("Fork error");
        }
        if (pid == 0)
        {

            close(fd[i][1]);

            srand(getpid());
            nrGenerated = rand() % 500 + 1;

            if (read(fd[i][0], &nrToCompare, sizeof(nrToCompare)) == -1)
            {
                perror("Read Error");
            }
            close(fd[i][0]);

            close(fd[i + 1][0]);

            if (nrToCompare > nrGenerated)
            {
                if (write(fd[i + 1][1], &nrToCompare, sizeof(nrToCompare)) == -1)
                {
                    printf("%d\n", i);
                    perror("Write Error2");
                }
                close(fd[i + 1][1]);
            }
            else
            {
                if (write(fd[i + 1][1], &nrGenerated, sizeof(nrGenerated)) == -1)
                {
                    perror("Write Error3");
                }
                close(fd[i + 1][1]);
            }

            exit(0);
        }
    }

    close(fd[0][0]);
    srand(getpid());
    nrGenerated = rand() % 500 + 1;

    if(printf("Pai: %d\n", nrGenerated)<0){
        perror("Printf error");
    }

    if (write(fd[0][1], &nrGenerated, sizeof(nrGenerated)) == -1)
    {
        perror("Write Error1");
    }
    close(fd[0][1]);
    for (i = 0; i < NR_PIPES - 1; i++)
    {
        wait(NULL);
    }

    close(fd[NR_PIPES - 1][1]);

    if (read(fd[NR_PIPES - 1][0], &nrGenerated, sizeof(nrGenerated)) == -1)
    {
        perror("Read Error1");
    }

    close(fd[NR_PIPES - 1][0]);

    if (printf("Maior número: %d", nrGenerated) < 0)
    {
        perror("Printf Error");
    }

    for (i = 0; i < NR_PIPES; i++)
    {
    }
    return 0;
}