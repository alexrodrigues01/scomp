#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#define PRODUCTS_NR 50

struct product
{
    int barCode;
    char name[50];
    int preco;
};

void generateProducts();

int main()
{
    struct product products[50];
    generateProducts(products);

    pid_t pid = -1;
    if (printf("BarCode: \n(Press -1 to exit)\n") < 0)
    {
        perror("Printf Error");
    }
    int code = 0, fd1[6][2], i;
    for (i = 0; i < 6; i++)
    {
        if (pipe(fd1[i]) == -1)
        {
            perror("Pipe error");
            return 1;
        }
    }
    for (i = 0; i < 5; i++)
    {
        if (pid != 0)
            pid = fork();

            if(pid==-1){
                perror("Fork error");
            }

        if (pid == 0)
        {

            struct product produto;
            close(fd1[0], [0]);
            for (;;)
            {
                scanf("%d", &code);

                if (write(fd1[0][1], &code, sizeof(code)) == -1)
                {
                    perror("Write Error1");
                }
                close(fd1[i + 1][1]);
                if (write(fd1[0][1], &i + 1, sizeof(code)) == -1)
                {
                    perror("Write Error2");
                }
                //       printf("code: %d i: %d\n",code,i);

                if (read(fd1[i + 1][0], &produto, sizeof(produto)) == -1)
                {
                    perror("Read Error3");
                }

                if(printf("Produto->\nNome:%s\nPreço:%d", produto.name, produto.preco)<0){
                    perror("Printf error");
                }
              
            }
            close(fd1[0], [1]);
            close(fd1[i + 1][0]);
            exit(0);
        }
    }

    // pai
    close(fd1[0][1]);
    int a;
    for (;;)
    {
        if (read(fd1[0][0], &code, sizeof(code)) == -1)
        {
            perror("Read Error4");
        }
        if (read(fd1[0][0], &a, sizeof(a)) == -1)
        {
            perror("Read Error5");
        }
        close(fd1[a][0]);
        for (i = 0; i < PRODUCTS_NR; i++)
        {
            if (products[i].barCode == code)
            {
                if(printf("code: %d a: %d\n", code, a)<0){
                    perror("Printf error");
                }

                if (write(fd1[a][1], &products[i], sizeof(products[i])) == -1)
                {
                    perror("Write Error");
                }
            }
        }
    
    }

    close(fd1[0][0]);
    close(fd1[a][1]);

    return 0;
}

void generateProducts(struct product products[])
{
    int i;
    srand(getpid());
    for (i = 0; i < PRODUCTS_NR; i++)
    {
        products[i].barCode = i + 1;
        sprintf(products[i].name, "Product: %d", i + 1);
        products[i].preco = i;
    }
}
