#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wait.h>
#include <time.h>
#include <string.h>

/*
*
*/

int main()
{
    sem_unlink("/semReader");
    sem_unlink("/read");
    sem_unlink("/write");
    sem_unlink("/semWriter");
    shm_unlink("/shm");
    return 0;
}
