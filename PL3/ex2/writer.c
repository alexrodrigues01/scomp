#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>


#define DESCRIPTION_SIZE 100


typedef struct{
    int productCode;
    char description[DESCRIPTION_SIZE];
    int price;
} product_struct;

int productCodeLido;
char descriptionLida[DESCRIPTION_SIZE];
int priceLido;


int main(int argc, char*argv[]) {
    int fd, data_size= sizeof(product_struct);
    product_struct *produto;
    fd=shm_open("/ex2", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
    ftruncate(fd,data_size);
    produto=(product_struct*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    scanf("%d",&productCodeLido);
    scanf("%s",descriptionLida);
    scanf("%d",&priceLido);

    produto->productCode=productCodeLido;
    strcpy(produto->description,descriptionLida);
    produto->price=priceLido;


    return 0;
}