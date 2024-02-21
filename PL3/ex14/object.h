#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#define EXCHANGES 30
#define NUMBERS 10

typedef struct {
    int head;
    int tail;
    int buffer[NUMBERS];
    int size;
} buffer;

void writeB(buffer *shm, int value);
void readB(buffer *shm, int *value);
int isFull(buffer *shm);
int isEmpty(buffer *shm);
