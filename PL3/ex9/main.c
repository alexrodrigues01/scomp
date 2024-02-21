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
  int i, size = sizeof(int) * 10, array[ARRAY_SIZE];
  srand(getpid());
  for (i = 0; i < ARRAY_SIZE; i++)
  {
    array[i] = random() % 1001;
  }


  int fd = shm_open("/shmint", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

  if (fd == -1)
  {
    perror("Opening shared memory ERROR.\n");
    exit(EXIT_FAILURE);
  }
  if (ftruncate(fd, size) == -1)
  {
    perror("Erro na alocação da memória.");
    return 0;
  }
  int *maximum = (int *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (maximum == MAP_FAILED)
  {
    perror("Error maping the object.\n");
    exit(EXIT_FAILURE);
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
      int a, max = 0;
      for (a =0; a < (i + 1) * 100; a=a+100)
      {
        if (array[i] > max)
        {
          max = array[i];
        }
      }
      maximum[i] = max;
      exit(0);
    }
  }

  for (i = 0; i < 10; i++)
  {
    wait(NULL);
  }

  int r = shm_unlink("/shmint"); /* removes */
  if (r < 0)
    exit(1);

  int max = 0;
  for (i = 0; i < 10; i++)
  {
    printf("%d\n",maximum[i]);
    if (maximum[i] > max)
    {
      max = maximum[i];
    }
  }

  printf("Maior valor: %d\n", max);
  
  return 0;
}
