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

    int fd[2],i;

    pid_t pid;

    /* cria o pipe */
    if (pipe(fd) == -1)
    {
        perror("Pipe failed");
        return 1;
    }

    pid = fork();

    if(pid==-1){
        perror("Fork error");
    }
    if (pid > 0)
    {
        if(printf("Enter a String and an integer :")<0){
            perror("Printf error");
        }
        scanf("%s %d", write_msg, &i);
        
        /* fecha a extremidade não usada */
        close(fd[0]);
        /* escreve no pipe */
        if(write(fd[1], write_msg, sizeof(read_msg))==-1){
            perror("Write error");
        }
       
        if(write(fd[1], &i, sizeof(i))==-1){
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
        if(read(fd[0], read_msg, sizeof(read_msg))==-1){
            perror("Read error");
        }
        if(read(fd[0],  &i, sizeof(i))==-1){
            perror("Read error");
        }
        
       
        
        
		
		if(printf("Filho leu: %s e %d",read_msg,i)==-1){
            perror("Printf error");
        }
        /* fecha a extremidade de leitura */
        close(fd[0]);
    }
    return 0;
}
