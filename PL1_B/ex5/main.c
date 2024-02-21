#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

void handle_SIGINT(int signo)
{
if(write(STDOUT_FILENO,"I won’t let the process end with CTRL-C!\n",43)<0){
	perror("Write error");
}
}

void handle_SIGQUIT(int signo)
{
if(write(STDOUT_FILENO,"I won’t let the process end with CTRL-\\!\n",50)<0){
	perror("Write error");
}
}


int main() {
	if(printf("%d",getpid())<0){
		perror("Printf error");
	}
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask); /* No signals blocked */
	act.sa_handler = handle_SIGINT;
	if(sigaction(SIGINT, &act, NULL)==-1){
		perror("Sigaction error");
	}
	
	struct sigaction act2;
	memset(&act2, 0, sizeof(struct sigaction));
	sigemptyset(&act2.sa_mask); /* No signals blocked */
	act2.sa_handler = handle_SIGQUIT;
	if(sigaction(SIGQUIT, &act2, NULL)==-1){
		perror("Sigaction error");
	}
	
	 for(;;){
		if(printf("I Like Signal\n")<0){
			perror("Printf error");
		}
	 sleep(1);
	 }
}
