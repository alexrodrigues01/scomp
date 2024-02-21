#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
 int a=0, b, c, d;
 b = (int) fork();
 if(b==-1){
     perror("Fork failed");
 }
 c = (int) getpid(); /* getpid(), getppid(): unistd.h*/
 d = (int) getppid();
 a = a + 5;
 if(printf("\na=%d, b=%d, c=%d, d=%d\n",a,b,c,d)<0){
     perror("Printf error");
 }
 }
