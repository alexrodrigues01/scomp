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
    int vec1[1000], vec2[1000], i, a, fd[5][2];

    pid_t pid;

    for (i = 1; i <= 1000; i++)
    {
        vec1[i - 1] = i;
        vec2[i - 1] = i;
    }

    for (i = 0; i < 5; i++)
    {
        /* cria o pipe */

        if (pipe(fd[i]) == -1)

        {

            perror("Pipe failed");

            return 1;
        }

        pid = fork();

        if (pid == -1)
        {
            perror("Fork Error");
        }

        if (pid == 0)
        {
            close(fd[i][0]);
            int soma = 0;
            for (a = i * 200; a < (i + 1) * 200; a++)
            {

                soma += vec1[a] + vec2[a];
            }

            if (write(fd[i][1], &soma, sizeof(soma)) == -1)
            {
                perror("Error");
            }

            close(fd[i][1]);
            exit(0);
        }
    }

    for (i = 0; i < 5; i++)
    {
        wait(NULL);
    }

    int somaFinal = 0;
    int somaParcial;
    for (i = 0; i < 5; i++)
    {
        close(fd[i][1]);
        while (read(fd[i][0], &somaParcial, sizeof(somaParcial)))
        {
            somaFinal += somaParcial;
        }

        close(fd[i][0]);
    }

    if(printf("Soma Final: %d", somaFinal)<0){
        perror("Printf error");
    }
    return 0;
}