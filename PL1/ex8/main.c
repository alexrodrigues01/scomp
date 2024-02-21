#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
int main()
 {
 int i;
 int status;

 for (i = 0; i < 4; i++) {
	 pid_t pid=fork();
	 if(pid==-1){
		 perror("Fork error");
	 }
	 if (pid == 0) {
		 exit(i+1); /*sleep(): unistd.h*/
	 }else{
		 if(pid%2==0){
			 waitpid(pid,NULL,0);
		 }
	 }
 }

 for (i = 0; i < 4; i++) {
	 wait(NULL);
 }

 if(printf("This is the end.\n")<0){
	 perror("Printf failed");
 }
 return 0;
 }
