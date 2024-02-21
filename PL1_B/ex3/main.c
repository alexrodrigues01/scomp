#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

void handle_USR1(int signo)
{
if(write(STDOUT_FILENO, "\nSIGUSR1 has been captured and was sent by the process with PID XX\n", 67)==-1){
	perror("Write error");
}
}


int main(int argc, char *argv[]){
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask); /* No signals blocked */
	act.sa_handler = handle_USR1;
	if(sigaction(SIGUSR1, &act, NULL)){
		perror("Sigaction error");
	}
    return 0;
 }
