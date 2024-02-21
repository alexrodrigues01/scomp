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


int main(int argc, char *argv[]){
	int fd, data_size = sizeof(student_struct);
	student_struct *student;
	fd = shm_open("/ex1", O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(fd, data_size);
	student = (student_struct*)mmap(NULL, data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	int r = shm_unlink("/ex1"); /* removes */
    if (r < 0) exit(1);
    if(printf ("Nome do aluno: %s\n", student->nome)<0){
		perror("Printf error");
	}
	if(printf ("Numero do aluno: %d\n", student->numero)<){
		perror("Printf error");
	}
	

	return 0;
}