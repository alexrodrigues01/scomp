#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>


#define NAME_SIZE 100


typedef struct{
    char nome[NAME_SIZE];
    int numero;
} student_struct;

char nomeLido[NAME_SIZE];
int numeroLido;


int main(int argc, char*argv[]) {
    int fd, data_size= sizeof(student_struct);
    student_struct *student;
    fd=shm_open("/ex1", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
    ftruncate(fd,data_size);
    student=(student_struct*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    scanf("%s",nomeLido);
    scanf("%d",&numeroLido);

    strcpy(student->nome,nomeLido);
    student->numero=numeroLido;


    return 0;
}