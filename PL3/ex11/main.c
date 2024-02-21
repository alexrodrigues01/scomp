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

int main()
{
  int i, array[ARRAY_SIZE], fd[2];
  srand(getpid());
  for (i = 0; i < ARRAY_SIZE; i++)
  {
    array[i] = random() % 1001;
  }

  if (pipe(fd) == -1)
  {
    perror("Pipe failed");
    return 1;
  }

  for (i = 0; i < 10; i++)
  {
    pid_t pid = -1;
    if (pid != 0)
    {
      pid = fork();
    }
    if (pid == -1)
    {
      perror("Erro no fork.");
    }

    if (pid == 0)
    {
      int a;
      int * max =malloc(sizeof(4));
      *max=0;

      for (a = 0; a < (i + 1) * 100; a = a + 100)
      {
        if (array[i] > *max)
        {
          *max = array[i];
        }
      }
      printf("%d\n",*max);
      close(fd[0]);
      write(fd[1], max, sizeof(int));
      close(fd[1]);
      exit(0);
    }
    
  }

  for (i = 0; i < 10; i++)
  {
    wait(NULL);
  }

  int max = 0, maximum;
  close(fd[1]);
  for (i = 0; i < 10; i++)
  {
    read(fd[0], &maximum, sizeof(int) );
    if(maximum>max){
      max=maximum;
    }
  }
  close(fd[0]);
  printf("Maior valor: %d\n", max);

  return 0;
}
