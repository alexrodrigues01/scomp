#include <sys/mman.h>
#include <sys/stat.h> /* For constants “mode” */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define VALUE 100
#define TIMES 1000000

typedef struct
{
    int number1;
} numbers;

int main()
{

    int fd, i;

    numbers *valores;

    fd = shm_open("/ex8", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, sizeof(numbers));
    valores = (numbers *)mmap(NULL, sizeof(numbers), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    valores->number1 = 8000;

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Fork error");
    }

    for (i = 0; i < TIMES; i++)
    {
        valores->number1 += 1;
        valores->number1 -= 1;
    }

    if (pid > 0)
    {
        if (printf("Pai: %d\n", valores->number1) < 0)
        {
            perror("Printf error");
        }
        wait(NULL);
    }
    else
    {
        if (printf("Filho: %d\n", valores->number1) < 0)
        {
            perror("Printf error");
        }
        exit(0);
    }

    int r = shm_unlink("/ex8"); /* removes */
    if (r < 0)
        exit(1);

    return 0;
}
