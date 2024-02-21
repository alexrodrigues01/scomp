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
	if(printf("SIGUSR1 signal captured, USR1_COUNTER =%d\n",USR1_COUNTER)<0){
		perror("Printf error");
	}
}

int main() {
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
