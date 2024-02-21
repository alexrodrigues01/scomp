#define ARRAY_SIZE 2000
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int main(){
	 int numbers[ARRAY_SIZE];
	 int i,j,nr=0;
	 time_t t;
	 srand ((unsigned) time (&t));
	 for (i = 0; i < ARRAY_SIZE; i++)
	 numbers[i] = rand () % 10000;
	 int n = rand () % 10000;

	 for(i=0;i<10;i++){
		 pid_t pid=fork();
		 if(pid==-1){
			 perror("Fork error");
		 }
		 if(pid==0){
			 int index=255;
			 for(j=0;j<200;j++){
				 if(numbers[j+nr]==n){
					 index=j+nr;
				 }
			 }
			 exit(index);
		 }else{
			 j+=200;
			 waitpid(pid);
			 if(printf("%d")<0){
				 perror("Printf failed");
			 }
		 }
	 }
	 return 0;
}
