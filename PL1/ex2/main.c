#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


int main(){
	int f;
	int n=3;
for (f=0; f < n; f++) {
	 pid_t pid = fork();

	 if(pid==-1){
		 perror("Fork error");
	 }
     if (pid==0){
    	 if(printf("I'll never join you!\n")<0){
			perror("Printf error");
		 }
    	 exit(0);
     }
else{
	if(f==0){

		if(printf("I'm\n")<0){
			perror("Printf error");
		}
	}else{
		if(f==1){
			if(printf("the\n")<0){
				perror("Printf error");
			}
		}else{
			if(printf("father!\n")<0){
				perror("Printf error");
			}
		}
	}

wait();
}
}
return 0;
}
