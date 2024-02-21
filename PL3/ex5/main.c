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

int main(int argc, char *argv[])
{
  int time = 0, fd;

  fd = shm_open("/shmobjeto", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
  if (fd == -1)
  {
    printf("Erro ao criar memória partilhada!");
  }
  if (ftruncate(fd, sizeof(struct object)) == -1)
  {
    perror("Erro na alocação da memória.");
    return 0;
  }
  struct object *objeto = (struct object *)mmap(NULL, sizeof(struct object *), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  objeto->num1 = 8000;
  objeto->num2 = 200;
  pid_t pid = fork();
  if (pid == -1)
  {
    perror("Erro no fork.");
  }
  if (pid == 0)
  {
    while (time != TIMES)
    {
      objeto->num1 -= 1;
      objeto->num2 += 1;
      time++;
    }
    exit(0);
  }
  else
  {
    while (time != TIMES)
    {
      objeto->num1 += 1;
      objeto->num2 -= 1;
      time++;
    }
  }
  wait(NULL);
  printf("Num1: %d\n", objeto->num1);
  printf("Num2: %d\n", objeto->num2);
  
  int r = shm_unlink("/shmobjeto"); /* removes */
  if (r < 0)
    exit(1);

  return 0;
}