#define ARRAY_SIZE 1000
#include <time.h>
#include <stdlib.h>
int main(){
	int i,a,limit=200,maxValue=0,status;
	 int numbers[ARRAY_SIZE];
	 time_t t;
	 srand ((unsigned) time (&t));

	 for (i = 0; i < ARRAY_SIZE; i++)
		 numbers[i] = rand () % 256;

	 for(i=1;i<=5;i++){
		 pid_t pid=fork();
		 if(pid==-1){
			 perror("Fork failed");
		 }
		 if(pid==0){
			 for(a=limit-200;a<limit;a++){
				 if(numbers[a]>maxValue){
					 maxValue=numbers[a];
				 }
			 }
			 if(printf("MaxValue filho %d: %d\n",i,maxValue)<0){
				 perror("Printf error");
			 }
			 exit(maxValue);
		 }
		 waitpid(pid,&status,0);
		 if(WEXITSTATUS(status)>maxValue){
			 maxValue=WEXITSTATUS(status);
		 }
		 limit+=200;
	 }

	 if(printf("MaxValue PAi : %d\n",maxValue)<0){
		 perror("Printf error");
	 }


	 int result[ARRAY_SIZE];
	 pid_t pid=fork();
	 if(pid==-1){
		 perror("Fork error");
	 }
	 if(pid==0){
		 for(int i=0;i<ARRAY_SIZE /2;i++){
			 result[i]=((int) numbers[i]/maxValue)*100;
		 }
		 exit(0);
	 }else{
		 for(int i=ARRAY_SIZE /2;i<ARRAY_SIZE ;i++){
		 		result[i]=((int) numbers[i]/maxValue)*100;
		 }
	 }

	 /*
	  * Print all the numbers
	  */
	 for(int i=0;i<ARRAY_SIZE ;i++){
	 	if(printf("%d\n",result[i])<0){
			 perror("Fork error");
		 }
	 }


}
