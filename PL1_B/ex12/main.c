#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>


int nrSIGUSR1,nrSIGUSR2;

void simulate1();
pid_t PID_child [50];


void handle_SIGUSR1(){
    nrSIGUSR1++;
    if(nrSIGUSR1+nrSIGUSR2==25){
           int i;
        for(i=0; i<50; i++){ 
			if(kill(PID_child[i],SIGUSR1)==-1){
                perror("Kill error");
            }
		}
    }
}

void handle_SIGUSR2(){
    nrSIGUSR2++;
    if(printf("SIGUSR2: %d\n",nrSIGUSR2)<0){
        perror("Printf error");
    }
    if(nrSIGUSR1+nrSIGUSR2==25){
         if(write(STDOUT_FILENO, "\nInefficient algorithm\n\n",24)==-1){
             perror("Write error");
         }
         // terminar todos os filhos
         int i;
        for(i=0; i<50; i++){ 
			if(kill(PID_child[i],SIGKILL)==-1){
                perror("Kill error");
            }
		}
    }
}

void children_handle_SIGUSR1(){
   execlp("./simulate2","./simulate2",NULL);
}

int main(void)
{
    nrSIGUSR1=0;
    nrSIGUSR2=0;
    int i;
    struct sigaction act,act2;

    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = handle_SIGUSR1;
    if(sigaction(SIGUSR1, &act, NULL)==-1){
        perror("Sigaction error");
    }

    memset(&act2, 0, sizeof(struct sigaction));
    sigemptyset(&act2.sa_mask);
    act2.sa_sigaction = handle_SIGUSR2;
    if(sigaction(SIGUSR2, &act2, NULL)==-1){
        perror("Sigaction error");
    }
    
    for (i = 0; i < 50; i++)
    {
		
        pid_t pid = fork();
        if(pid==-1){
            perror("Fork error");
        }
        if (!pid)
        {
			
			struct sigaction act3;
			
			memset(&act3, 0, sizeof(struct sigaction));
			sigemptyset(&act3.sa_mask);
			act3.sa_sigaction = children_handle_SIGUSR1;
			if(sigaction(SIGUSR1, &act3, NULL)==-1){
                perror("Sigaction error");
            }
    
            simulate1();
            
            pause();
            
        }else{PID_child[i]=pid;}
		
    }
    for (i = 0; i < 50; i++)
    {
       wait(NULL);
    }
    
    return 0;
}

void simulate1()
{
    if(printf("Simulate 1 - ")<0){
        perror("Printf error");
    }
    
    srand(time(NULL) ^ getpid());
    
    int randd = rand() % 2;
     if(printf("%d\n\n",randd)<0){
         perror("Printf error");
     }
    if (randd)
    {
        if(kill(getppid(), SIGUSR1)==-1){
            perror("Kill error");
        }
    }
    else
    {
        if(kill(getppid(), SIGUSR2)==-1{
            perror("Kill error");
        }
    }
}
