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
    int fd, size = sizeof(char)*ARRAY_SIZE,i;
      shm_unlink("/shmchar"); /* removes */
    fd = shm_open("/shmchar", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, size);
    char * array = (char *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);  
    srand(getpid());
    for(i=0;i<ARRAY_SIZE;i++){
        array[i]= 'A' + (random() % 26);
    }
    return 0;
}
