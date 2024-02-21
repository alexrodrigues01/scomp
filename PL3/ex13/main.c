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
  int i, size = sizeof(struct info) * SONS;

  int fd = shm_open("/shmfile", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

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
  struct info *array = (struct info *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (array == MAP_FAILED)
  {
    perror("Error maping the object.\n");
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < 10; i++)
  {
    pid_t pid = fork();

    if (pid == -1)
    {
      perror("Erro no fork.");
    }

    if (pid > 0)
    {
      char fileName[50];
      sprintf(fileName, "ficheiro%d.txt", i + 1);
      strcpy(array[i].path, fileName);
     strcpy( array[i].word , "Ola");
      array[i].nrOccurrences = 0;
      array[i].read = 1;
    }
    else
    {
      while (array[i].read != 1)
      {
      }
      printf("%s\n",array[i].path);
      FILE *fptr;
      if ((fptr = fopen(array[i].path, "r")) == NULL)
      {
        printf("Error! opening file");
        // Program exits if the file pointer returns NULL.
        exit(1);
      }
      char line[100];
      fscanf(fptr, "%[^\n]\n" , line);
      printf("%s\n",line);
      int a, count = 0, j = 0;
      for (a = 0; a < strlen(line); a++)
      {
        if (line[a] == array[i].word[j])
        {
          count++;
          j++;
          if (count == strlen(array[i].word))
          {
            array[i].nrOccurrences++;
          }
        }
        else
        {
          j=0;
          count = 0;
        }
      }
      fclose(fptr);
      exit(0);
    }
  }
  for(i=0;i<SONS;i++){
    wait(NULL);
  }
 for(i=0;i<SONS;i++){
   printf("Find the word: %s in the file '%s'\nNr: %d\n\n ",array[i].word,array[i].path,array[i].nrOccurrences);
 }
  int r = shm_unlink("/shmfile"); /* removes */
  if (r < 0)
  {
    perror("Eliminar memoria.\n");
    exit(1);
  }
  return 0;
}
