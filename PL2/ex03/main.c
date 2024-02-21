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

    char msg[80];
    
    int fd[2],status;

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

    if (pid > 0) {
         /* fecha a extremidade não usada */
        close(fd[0]);
        strcpy(msg,"Hello World");


        if(write(fd[1],msg,sizeof(msg))==-1){
            perror("Write error");
        }
        strcpy(msg,"Goodbye!");


        if()
        if(write(fd[1],msg,sizeof(msg))==-1){
            perror("Write error");
        }

         /* fecha a extremidade não usada */
        close(fd[1]);
        waitpid(pid,&status,0);

        if(printf("Pai: o filho %d retornou o valor: %d \n", pid, WEXITSTATUS(status))<0){
            perror("Printf error");
        }

    }else{
         /* fecha a extremidade não usada */
        close(fd[1]);
    
        if(read(fd[0],msg,sizeof(msg))==-1){
            perror("Read error");
        }

        if(printf("%s\n",msg)<0){
            perror("Printf error");
        }
        strcpy(msg,"");

        if(read(fd[0],msg,sizeof(msg))==-1){
            perror("Read error");
        }

        
        if(printf("%s\n",msg)<0){
            perror("Printf error");
        }
        
         /* fecha a extremidade não usada */
        close(fd[0]);


        if(printf("Filho acabou\n")<0){
            perror("Printf error");
        }
        exit(0);
    }
    return 0;
}
