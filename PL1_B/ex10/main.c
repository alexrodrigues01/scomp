

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
    if (strlen(string) == 0)
    {
        if(write(STDOUT_FILENO, "To slow, that is why the program will end!\n", 44)==-1){
            perror("Write error");
        }
        exit(0);
    }
}

int main()
{

    struct sigaction act;
    act.sa_handler = handle_signal;
    if(sigaction(SIGALRM, &act, NULL)==-1){
        perror("Sigaction error");
    }

    alarm(10); // Scheduled alarm after 10 seconds

    if(printf("Write something: ")<0){
        perror("Printf error");
    }
    scanf("%s", string);

    sleep(20);

    if(printf("Successful execution!")<0){
        perror("Printf error");
    }

    return 0;
}
