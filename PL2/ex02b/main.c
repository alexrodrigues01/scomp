#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#define BUFFER_SIZE 80

struct mensagem{
	char texto [80] ;
	int number;
};

int main()
{


    char write_msg[BUFFER_SIZE];

    int fd[2],i;
	
    pid_t pid;
	
	struct mensagem mensagem1;
	
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
        
        strcpy(mensagem1.texto,write_msg);
        mensagem1.number=i;
        
        /* escreve no pipe */
        if(write(fd[1],&mensagem1, sizeof(mensagem1))==-1){
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
        
        if(read(fd[0], &mensagem1, sizeof(mensagem1))==-1){
            perror("Read error");
        }
        
       
		if(printf("Filho leu: %s e %d",mensagem1.texto,mensagem1.number)){
            perror("Printf error");
        }
        /* fecha a extremidade de leitura */
        close(fd[0]);
    }
    return 0;
}
