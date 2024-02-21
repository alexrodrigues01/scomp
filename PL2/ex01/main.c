#include <stdio.h>

#include <unistd.h>

#include <sys/types.h>

#include <signal.h>

#include <string.h>

#include <stdlib.h>

#include <time.h>

#include <sys/wait.h>

#define BUFFER_SIZE 80

int main()

{

    char read_msg[BUFFER_SIZE];

    char write_msg[BUFFER_SIZE];

    int fd[2];

    pid_t pid;

    /* cria o pipe */

    if (pipe(fd) == -1)
    {

        perror("Pipe failed");

        return 1;
    }

    pid = fork();

    if (pid > 0)

    {

        sprintf(write_msg, "Pid Pai: %d", getpid());

        /* fecha a extremidade não usada */
        close(fd[0]);
        if(write(STDOUT_FILENO, write_msg, strlen(write_msg) + 1)==-1){
            perror("Write error");
        }

        /* escreve no pipe */

        if(write(fd[1], write_msg, strlen(write_msg) + 1)==-1){
            perror("Write error");
        }

        /* fecha a extremidade de escrita */

        close(fd[1]);

        wait(NULL);
    }

    else

    {

        /* fecha a extremidade não usada */

        close(fd[1]);

        /* lê dados do pipe */

        if(read(fd[0], read_msg, BUFFER_SIZE)==-1){
            perror("Read error");
        }

        if(printf("Filho leu: %s", read_msg)<0){
            perror("Printf error");
        }

        /* fecha a extremidade de leitura */

        close(fd[0]);
    }

    return 0;
}