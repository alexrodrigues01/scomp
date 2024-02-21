

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

char string[5000];

void handle_signal()
{
	if(printf("handling\n")<0){
        perror("Printf error");
    }
    if (strlen(string) == 0)
    {
        if(write(STDOUT_FILENO, "To slow, that is why the program will end!\n", 44)==-1){
            perror("Write error");
        }
        exit(0);
    }
    if(printf("handling\n")<0){
        perror("Printf error");
    }
}

int main()
{
    pid_t pid=fork();

    if(pid==-1){
        perror("Fork error");
    }

    // Filho
    if(pid==0){
        sleep(5);
        kill(getppid(),SIGUSR1);
        exit(0);
    }
	
    struct sigaction act;
    act.sa_handler = handle_signal;
    sigaction(SIGUSR1, &act, NULL);

    if(printf("Write something: ")<0){
        perror("Printf error");
    }
    scanf("%s", string);

	if(kill(pid,SIGKILL)==-1){// Termina-se o filho por não haver necessidade de continuar o processo
        perror("Kill error");
    } 
	
	sleep(10);

    if(printf("Successful execution!")<0){
        perror("Printf error");
    }

    return 0;
}
