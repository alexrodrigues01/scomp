#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

char FILE_NAME []="ficheiro.txt";

 int main(){
    int fd[2];

    pid_t pid;

    FILE * file;

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

    /* pai */
    if(pid>0){
        close(fd[0]);
        file = fopen(FILE_NAME, "r");
        int size;
        char buffer [50];
        while( (size=fread(buffer, 1 , 8, file))>0 ){
            if(write(fd[1],buffer,size)==-1){
                perror("Write error");
            }
        }

        /* fecha a extremidade não usada */
        close(fd[1]);

        wait(NULL);

    }else{
        /* fecha a extremidade não usada */
        close(fd[1]);
        
        char readmsg [50];
        while (read(fd[0], readmsg, 8)){
			if(printf("%s", readmsg)<0){
                perror("Printf error");
            }  
		}
		exit(0);

    }
    return 0;
 }