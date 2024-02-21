#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

volatile sig_atomic_t childrenEnded;

void handle_signal()
{
    childrenEnded++;
    if(write(STDOUT_FILENO, "A child has ended...\n", 22)==-1){
        perror("Write error");
    }
}

int main()
{
    int i;
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));

    sigfillset(&act.sa_mask);
    act.sa_handler = handle_signal;
    act.sa_flags = SA_SIGINFO | SA_NOCLDWAIT | SA_NOCLDSTOP;

    if(sigaction(SIGCHLD, &act, NULL)){
        perror("Sigaction error");
    }

    for (i = 0; i < 5; i++)
    {
        pid_t pid = fork();

        if(pid==-1){
            perror("Fork error");
        }

        if (pid == 0)
        {
            int a;
            for (a = i * 200; a < (i + 5) * 200; a++)
            {
             //   printf("%d ", a);
            }
            if(printf("\n ")<0){
                perror("Printf error");
            }
            exit(0);
        }
    }
    
 

    while (childrenEnded != 5)
    {
        pause();
    }
    
    for(i=0;i<5;i++){
	 wait(NULL);
	}
	
    if(printf("Ended!")<0){
        perror("Printf error");
    }
	return 0;
}
