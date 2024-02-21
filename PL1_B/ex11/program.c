#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

int main (int argc, char *argv[]){
    printf("Name: %s",argv[0]);
    sleep(atoi(argv[1]));
    return 0;
}
