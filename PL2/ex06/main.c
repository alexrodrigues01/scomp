#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

int main()
{
    int vec1[1000], vec2[1000], i, fd[2], a;
    pid_t pid;

    for (i = 1; i <= 1000; i++)
    {
        vec1[i - 1] = i;
        vec2[i - 1] = i;
    }

    /* cria o pipe */

    if (pipe(fd) == -1)
    {

        perror("Pipe failed");

        return 1;
    }

    for (i = 0; i < 5; i++)
    {
        pid = fork();

        if(pid==-1){
            perror("Fork error");
        }
        if (pid == 0)
        {
            close(fd[0]);
            int soma = 0;
            for (a = i * 200; a < (i + 1) * 200; a++)
            {

                soma += vec1[a] + vec2[a];
            }

            if(write(fd[1], &soma, sizeof(soma))==-1){
                perror("Write error");
            }
            close(fd[1]);
            exit(0);
        }
    }
    
    for (i = 0; i < 5; i++)
    {
        wait(NULL);
    }
    close(fd[1]);
    int somaFinal = 0;
    int somaParcial;
    while (read(fd[0], &somaParcial, sizeof(somaParcial)))
    {
       somaFinal+=somaParcial;
    }

    if(printf("Soma Final: %d",somaFinal)<0){
        perror("Printf error");
    }
     close(fd[0]);
     return 0;
}