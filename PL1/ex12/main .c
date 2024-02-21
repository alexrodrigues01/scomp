#define ARRAY_SIZE 1000
#include <time.h>
#include <stdlib.h>

int spawn_childs(int n) {

	int i;
	for(i=1;i<=n;i++){
		pid_t pid=fork();
		if(pid==-1){
			perror("Fork error");
		}
		if(pid==0){
			return i;
		}
		wait();
	}
	return 0;
}


int main(){
	if(printf("%d\n",spawn_childs(6))<0){
		perror("Printf error");
	}
	return 0;
}
