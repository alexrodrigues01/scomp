#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void){
	int out1,out2;
	pid_t pid1=fork();
	if(pid1==-1){
		perror("Fork error");
	}
	waitpid(pid1,&out1,0);
	if(pid1==0){
		sleep(1);
		if(printf("Filho1")<0){
			perror("Printf error");
		}
		exit(1);
	}else{
		pid_t pid2=fork();
		if(pid2==-1){
			perror("Fork error");
		}
		waitpid(pid2,&out2,0);
		if(pid2==0){
			sleep(2);
			if(printf("Filho2")<0){
			perror("Printf error");
		}
			exit(2);
		}

	if(printf("Pai\n\n")<0){
		perror("Printf error");
	}
	if(WIFEXITED(out1) && WIFEXITED(out2)){
		if(printf("SON 1 return value = %d\n", WEXITSTATUS(out1))<0){
			perror("Printf failed");
		}
		if(printf("SON 2 return value = %d\n", WEXITSTATUS(out2))<0){
			perror("Printf failed");
		}
	}

	}
	return 0;
}
