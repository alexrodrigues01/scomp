#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

int main(){
	int i,j;
	int nr=1;
	for(i=1;i<=10;i++){
		pid_t p=fork();
		if(p==-1){
			perror("Fork error");
		}
		if(p==0){
			for(j=nr;j<nr+100;j++){
				if(printf("%d ",j)<0){
					perror("Printf error");
				}
			}
			if(printf("\n")<0){
				perror("Printf error");
			}
			exit(0);
		}else{
			nr+=100;
			waitpid(p);
		}

	}
}
