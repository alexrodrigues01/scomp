#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#define SIZE 50000
#define criteria 20

typedef struct
{
    int customer_code;
    int product_code;
    int quantity;
} sale;

void randomSale();

int main()
{
    int fd[2], i, products[SIZE];
    sale sales[SIZE];
    pid_t pid = -1;

    if (pipe(fd) == -1)
    {

        perror("Pipe failed");

        return 1;
    }
    randomSale(sales);
    for (i = 0; i < 10; i++)
    {
        if (pid != 0)
        {
            pid = fork();
            if(pid==-1){
                perror("Fork error");
            }
        }
        else
        {
            break;
        }
    }

    // filho
    if (pid == 0)
    {
        close(fd[0]);
        for (i = 0; i < SIZE; i += 5000)
        {
            if (sales[i].quantity > criteria)
            {

                if (write(fd[1], &sales[i].product_code, sizeof(sales[i].product_code)) == -1)
                {
                    perror("Write Error");
                }
            }
        }
        exit(0);
        close(fd[1]);
    }

    for (i = 0; i < 10; i++)
    {
        wait(NULL);
    }

    close(fd[1]);
    int code;
    i = 0;

    while (read(fd[0], &code, sizeof(code)))
    {
        products[i] = code;
        if (printf("Product: %d\n", products[i]) < 0)
        {
            perror("Printf Error");
        }
        i++;
    }

    close(fd[0]);

    return 0;
}

void randomSale(sale sales[])
{
    int i;
    time_t t;
    /* Intializes random number generator */
    srand((unsigned)time(&t));
    for (i = 0; i < SIZE; i++)
    {
        sales[i].customer_code = rand() % 11;
        sales[i].product_code = rand() % 11;
        sales[i].quantity = rand() % 50;
    }
}