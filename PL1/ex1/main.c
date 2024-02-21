#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
 int x = 1;
 pid_t p = fork(); /*pid_t: sys/types.h; fork(): unistd.h*/
 if(p==-1){
	 perror("Fork error");
 }
 if (p == 0) {
	 x = x+1;

	 if(printf("1. x = %d\n", x)<0){
		 perror("Printf error");
	 }
 } else {
	 x = x-1;
	 
	 if(printf("2. x = %d\n", x)<0){
		 perror("Printf error");
	 };
 }
if(printf("3. %d; x = %d\n", p, x)<0){
	perror("Printf error");
};
return 0;
} 
