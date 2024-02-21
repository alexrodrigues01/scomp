#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>


void sendSignal(){
    int signal;
    pid_t pid;
    scanf("%d",&signal);
    scanf("%d",&pid);
    if(kill(pid,signal)==-1){
        perror("Kill error");
    }
    }



int main(){

	if(printf("%d\n",getpid())<0){
        perror("Printf error");
    }
    sendSignal();
    for(;;){
        if(printf("I love SCOMP!\n")<0){
            perror("Printf error");
        }
        sleep(1);

    }
}


