#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

int MSG_SIZE = 256;

int main()
{
    int up[2], down[2],i;
    char input[MSG_SIZE];
    char output[MSG_SIZE];
    pid_t pid;

    if (pipe(up) == -1 || pipe(down) == -1)
    {

        perror("Pipe failed");

        return 1;
    }

    pid = fork();

    if(pid==-1){
        perror("Fork error");
    }

    // Filho - Client
    if (pid == 0)
    {
        close(down[1]);
        close(up[0]);

        if(printf("Introduza uma string: ")<0){
            perror("Printf error");
        }
        fgets(input, MSG_SIZE, stdin);
        
        if(write(up[1], input, MSG_SIZE)==-1){
            perror("Write error");
        }

        close(up[1]);

        waitpid(getppid(), NULL, 0);

        if(read(down[0], output, MSG_SIZE)==-1){
            perror("Read error");
        }

        close(down[0]);

        if(printf("Resposta do server: %s", output)<0){
            perror("Printf error");
        }
    }
    else
    {
        close(down[0]);

        close(up[1]);
        // Pai - Server

        if(read(up[0], output, MSG_SIZE)==-1){
            perror("Read error");
        }

        close(up[0]);


        for (i = 0; output[i] != '\0'; i++)
        {
            if (output[i] >= 'a' && output[i] <= 'z')
            {
                output[i] = output[i] - 32;
            }
            else if (output[i] >= 'A' && output[i] <= 'Z')
            {
                output[i] = output[i] + 32;
            }
        }
 

        if(write(down[1], output, MSG_SIZE)==-1){
            perror("Write error");
        }

        close(down[1]);
        exit(0);
    }

    return 0;
}
