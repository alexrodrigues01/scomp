#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

struct win
{
    char msg[3];
    int round;
};

int main()
{

    if(printf("Win the pipe!\n")<0){
            perror("Printf Error");
    }

    int fd[2], i, pids[10];
    pid_t pid = -1;

    if (pipe(fd) == -1)
    {

        perror("Pipe failed");

        return 1;
    }

    for (i = 0; i < 10; i++)
    {
        if (pid != 0)
        {
            pid = fork();
            if(pid==-1){
                perror("Fork error");
            }
            pids[i] = pid;
        }
    }

    struct win winPipe;
    if (pid != 0)
    {
        for (i = 1; i <= 10; i++)
        {

            strcpy(winPipe.msg, "Win");
            winPipe.round = i;
           
            close(fd[0]);

            if (write(fd[1], &winPipe, sizeof(winPipe)) == -1)
            {
                perror("Write Error");
            }

           
        }
         close(fd[1]);
    }
    else
    {
        if (read(fd[0], &winPipe, sizeof(winPipe)) == -1)
        {
            perror("Read Error");
        }
        if(printf("%s -- Round %d\n\n",winPipe.msg,winPipe.round)<0){
            perror("Printf Error");
        }

        exit(winPipe.round);
    }
    int status[i];

    for (i = 0; i < 10; i++)
    {
       if( waitpid(pids[i], &status[i], 0)==-1){
            perror("Wait Pid Error");
       }
    }

    for (i = 0; i < 10; i++)
    {
        if(WIFEXITED(status[i])){
            if(printf("\nPID FILHO: %d\nWinning Round: %d\n\n",pids[i],WEXITSTATUS(status[i]))<0){
                perror("Printf error");
            }
        }
    }

    return 0;
}