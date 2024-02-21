#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

volatile sig_atomic_t taskAEnded=0;

void taskA();
void taskB();
void taskC();
void timer(int segundos,char task);

void handle_signal()
{
    taskAEnded=1;
    char string [100]="The signal SIGUSR1 has arrived!" ;
    if(write(STDOUT_FILENO, string, 100)){
        perror("Write error");
    }
    taskC();
}

int main()
{

    pid_t pid = fork();

    if(pid==-1){
        perror("Fork error");
    }

    if (pid != 0)
    {
        taskA();
        if(kill(pid, SIGUSR1)==-1){
            perror("Kill error");
        }
    }
    else
    {
        struct sigaction act;
        memset(&act, 0, sizeof(struct sigaction));

        sigfillset(&act.sa_mask);
        act.sa_handler = handle_signal;
        act.sa_flags = SA_SIGINFO | SA_NOCLDWAIT | SA_NOCLDSTOP;

        if(sigaction(SIGUSR1, &act, NULL)){
            perror("Sigaction error");
        }

        taskB();

        while(taskAEnded==0){}
    }
    wait(NULL);
    return 0;
}

void taskA()
{
    timer(3,'A');
}

void taskB()
{
    time_t t;
    /* Intializes random number generator */
    srand((unsigned)time(&t));
    timer(rand() % 6,'B');
}

void timer(int segundos, char task)
{
    int i;
    for (i = 1; i <= segundos; i++)
    {
        if(printf("%ds doing task %c\n", i,task)<0){
            perror("Printf error");
        }
        sleep(1);
    }
}

void taskC(){

    if(printf("Task C done! \n")<0){
        perror("Printf error");
    }
}

