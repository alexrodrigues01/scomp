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



int main(int argc, char *argv[]){
	int fd, data_size = sizeof(product_struct);
	product_struct *produto;
	fd = shm_open("/ex2", O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(fd, data_size);
	produto = (product_struct*)mmap(NULL, data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	int r = shm_unlink("/ex2"); /* removes */
    if (r < 0) exit(1);
    if(printf ("Codigo do produto: %d\n", produto->productCode)<0){
		perror("Printf error");
	}
	printf(("Descricao do produto: %s\n",produto->description)<0){
			perror("Printf error");
	}
	printf (("Preco do produto: %d\n", produto->price)<0){
			perror("Printf error");
	}
	

	return 0;
}