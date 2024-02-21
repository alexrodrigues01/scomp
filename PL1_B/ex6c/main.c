#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

volatile sig_atomic_t USR1_COUNTER;

void  handle_signal()
{
	USR1_COUNTER++; 
		char  phrase[80];
	sprintf(phrase,"SIGUSR1 signal captured, USR1_COUNTER =%d\n",USR1_COUNTER);
	if(write(STDOUT_FILENO, phrase, strlen(phrase))==-1){
		perror("Write error");
	}
}

int main() {


	pid_t pid=fork();

	if(pid==-1){
		perror("Fork error");
	}
	if(pid==0){
		struct timespec ts;
    
    	ts.tv_sec = 0;
    	ts.tv_nsec = 10000000 ;
    
		int i;
		for(i=0;i<12;i++){
			if(kill(getppid(),10)==-1){
				perror("Kill error");
			}
			nanosleep(&ts, NULL);
			if(printf("Mandou %d\n",i)<0){
				perror("Printf error");
			}
		}
		exit(0);
	}


	USR1_COUNTER=0;
	if(printf("%d\n\n",getpid())<0){
		perror("Printf error");
	}
	
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask); /* No signals blocked */
	act.sa_handler =  handle_signal;
	
	if(sigaction(SIGUSR1, &act, NULL)==-1){
		perror("Sigaction error");
	}
	 
	for(;;){
	if(printf("I’m working!\n")<0){
		perror("Printf error");
	}
	sleep(1);
	}

}
