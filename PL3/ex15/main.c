#include "object.h"

#define BUFFER_SIZE 10

int main() {
    // Criar pipe
    int fd[2];
    if (pipe(fd) == -1) {
        perror("Pipe failed");
        return 1;
    }

    int i, j, a = 1;
    int buffer[BUFFER_SIZE];
    pid_t pid = fork();
    if (pid == -1) {
		printf("Erro no fork.");
	}
    if (pid > 0) {
        close(fd[0]);

        for (i = 0; i < 3; i++) {
            for (j = 0; j < BUFFER_SIZE; j++) {
                buffer[j] = a;
                a++;
            }
            if(write(fd[1], &buffer, sizeof(int) * BUFFER_SIZE)==-1){
                 perror("Write Error");
            }
        }

         if (wait(NULL) == -1)
            perror("Error waiting");


        close(fd[1]);
    } else {
        close(fd[1]);

        for (i = 0; i < 3; i++) {
            if(read(fd[0], &buffer, sizeof(int) * BUFFER_SIZE)==-1){
                 perror("Read Error");
            }
            for (j = 0; j < BUFFER_SIZE; j++)
                printf("%d\a", buffer[j]);
        }

        close(fd[0]);
        exit(0);
    }
}