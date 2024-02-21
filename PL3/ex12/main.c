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
  int i, size = sizeof(struct aluno);

  pid_t pid = fork();

  if (pid == -1)
  {
    perror("Erro no fork.");
  }
  if (pid == 0)
  {
    int fd = shm_open("/shmaluno", O_RDWR, S_IRUSR | S_IWUSR);

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
    struct aluno *pessoa = (struct aluno *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (pessoa == MAP_FAILED)
    {
      perror("Error maping the object.\n");
      exit(EXIT_FAILURE);
    }
    while (pessoa->read != 1)
    {
    }

    int soma = 0, highest = 0, lowest = 10;

    for (i = 0; i < NR_DISC; i++)
    {
      int nr = pessoa->disciplinas[i];
       soma += nr;
      if (nr > highest)
      {
        highest = nr;
      }
      if (nr < lowest)
      {
        lowest = nr;
      }
    }
    printf("Highest: %d\nLowest: %d\nAverage: %d\n", highest, lowest, soma / NR_DISC);
    exit(0);
  }
  else
  {
    int fd = shm_open("/shmaluno", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

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
    struct aluno *pessoa = (struct aluno *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (pessoa == MAP_FAILED)
    {
      perror("Error maping the object.\n");
      exit(EXIT_FAILURE);
    }
    for (i = 0; i < 10; i++)
    {
      pessoa->disciplinas[i] = i+1;
    }
    strcpy(pessoa->nome, "Antonio");
    pessoa->numero = 23;
    pessoa->read = 1;
  }
  wait(NULL);
  int r = shm_unlink("/shmaluno"); /* removes */
  if (r < 0){
    perror("Eliminar memoria.\n");
    exit(1);}
  return 0;
}
