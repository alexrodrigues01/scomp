#include <sys/mman.h>
#include <sys/stat.h> /* For constants “mode” */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "char.h"
#include <stdlib.h>

int main()
{
  int fd, size = sizeof(char)*ARRAY_SIZE,i,soma=0;
  fd = shm_open("/shmchar", O_RDWR, S_IRUSR | S_IWUSR);
  ftruncate(fd, size);
  char  * array =( char * )mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  printf("Printing all chars");
  for(i=0;i<ARRAY_SIZE;i++){
    printf("%c\n",array[i]);
    soma+=array[i]; 
  }
  printf("Average: %d.\nChar: %c.\n",soma/ARRAY_SIZE,soma/ARRAY_SIZE);
    int r = shm_unlink("/shmchar"); /* removes */
    if (r < 0) exit(1); 
    return 0;
}