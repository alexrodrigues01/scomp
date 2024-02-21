#include <sys/mman.h>
#include <sys/stat.h> /* For constants “mode” */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "data.h"
#include <stdlib.h>

int main()
{
  int fd, size = sizeof(int) * ARRAY_SIZE, i;
  fd = shm_open("/shmint", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
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
  int *array = (int *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (array == MAP_FAILED)
  {
    perror("Error maping the object.\n");
    exit(EXIT_FAILURE);
  }

  srand(getpid());

  for (i = 0; i < ARRAY_SIZE; i++)
  {
    array[i] = random() % 21 + 1;
  }
  return 0;
}
