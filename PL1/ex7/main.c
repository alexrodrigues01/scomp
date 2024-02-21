#define ARRAY_SIZE 1000
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int main(void){
	 int numbers[ARRAY_SIZE]; /* array to lookup */
	 int n; /* the number to find */
	 time_t t; /* needed to init. the random number generator (RNG)
	*/
	 int i;

	 /* intializes RNG (srand():stdlib.h; time(): time.h) */
	 srand ((unsigned) time (&t));

	 /* initialize array with random numbers (rand(): stdlib.h) */
	 for (i = 0; i < ARRAY_SIZE; i++)
	 numbers[i] = rand () % 10000;

	 /* initialize n */
	 n = rand () % 10000;

	 if(printf("%d\n",n)<0){
		 perror("Printf error");
	 }
	 char time=0;int out;
	 pid_t pid=fork();
	 if(pid==-1){
		 perror("Fork error");
	 }
	 waitpid(pid,&out,0);



	 if(pid==0){
		 for(i=0;i<ARRAY_SIZE/2;i++){
			 if(numbers[i]==n){
				 time++;
			 }
		 }
		 exit(time);
	 }else{
		 for(i=ARRAY_SIZE/2;i<ARRAY_SIZE;i++){
			 if(numbers[i]==n){
				 time++;
			 }
		}
	 }
		if(printf("Number %d appeared %d times \n",n,WEXITSTATUS(out)+time)<0){
			perror("Printf error");
		}

	return 0;
}
