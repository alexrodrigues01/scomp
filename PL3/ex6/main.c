#include <sys/mman.h>
#include <sys/stat.h> /* For constants “mode” */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "object.h"
#include <sys/wait.h>
#include <sys/time.h>

void handle_signal() {}

long getMicrotime()
{ //returns the current time in microseconds
  struct timeval currentTime;
  gettimeofday(&currentTime, NULL);
  return currentTime.tv_sec * 1000000 + currentTime.tv_usec;
}

int main()
{

  int arrayInt[ARRAY_SIZE], i;
  int *arrayIntCopy, arrayDoubleCopy;

  for (i = 0; i < ARRAY_SIZE; i++)
  {
    arrayInt[i] = i;
  }

  int fd[2];

  pid_t pid;

  /* cria o pipe */
  if (pipe(fd) == -1)
  {
    perror("Pipe failed");
    return 1;
  }

  // Char

  long start = getMicrotime();

  if ((pid = fork()) > 0)
  {
    close(fd[0]);
    for (i = 0; i < ARRAY_SIZE; i++)
    {
      write(fd[1], &arrayInt[i], sizeof(int));
    }
    close(fd[1]);
    wait(NULL);
  }
  else
  {
    arrayIntCopy = (int *)malloc(ARRAY_SIZE * sizeof(int));
    close(fd[1]);
    for (i = 0; i < ARRAY_SIZE; i++)
    {
      read(fd[0], &arrayIntCopy[i], sizeof(int));
    }
    close(fd[0]);
    free(arrayIntCopy);
    exit(0);
  }

  long end = getMicrotime() - start;
  printf("Tempo Pipe: %ld\n", end);

  printf("SHARED MEMORY\n");
  // Shared Memory

  int fd2;
  pid = fork();
  if (pid == -1)
  {
    perror("Erro no fork.");
  }
  if (pid > 0)
  {
    printf("pai");
    start = getMicrotime();
    fd2 = shm_open("/shmFaster", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (ftruncate(fd2, ARRAY_SIZE * sizeof(int)) == -1)
    {
      perror("Erro na alocação da memória.");
      return 0;
    }
    int *array2 = (int *)mmap(NULL, ARRAY_SIZE * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
    for (i = 0; i < ARRAY_SIZE; i++)
    {
      array2[i] = arrayInt[i];
    }
    sleep(1);
    kill(pid, SIGUSR1);
    wait(NULL);
  }
  else
  {
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));

    sigfillset(&act.sa_mask);

    act.sa_handler = handle_signal;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);

    pause(); // espera que o pai escreva no shared memory

    fd2 = shm_open("/shmFaster", O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd2, ARRAY_SIZE * sizeof(int));
    arrayIntCopy = (int *)mmap(NULL, ARRAY_SIZE * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
    int r = shm_unlink("/shmFaster"); /* removes */
    if (r < 0)
      exit(1);
    exit(0);
  }
  end = getMicrotime() - start - 1000000;
  printf("Tempo Shared Memory: %ld\n", end);


  return 0;
}
