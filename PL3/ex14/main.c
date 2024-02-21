#include "object.h"

int main(void) {
	
    // inicializar memoria

	int i;

    int fd, data_size = sizeof(buffer);

	shm_unlink ("/shm_ex14");
	
	fd = shm_open("/shm_ex14", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
    if (fd == -1){
		perror("Erro ao criar memória partilhada!");
		return 1;
	}

	if (ftruncate (fd, data_size) == -1) {
        perror("Erro na alocação da memória.");
        return 1;
    }
    
	buffer *shm = (buffer*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    if (shm == MAP_FAILED || shm == 0) {
        perror("Erro ao mapear a memória.");
        return 0;
    }

    // inicializar valores do buffer
    shm->size = NUMBERS;
    shm->head = 0;
    shm->tail = 0;

    pid_t pid = fork();

	if (pid == -1) {
		printf("Erro no fork.");
	}
	
    if (pid == 0) {

        for (i = 0; i < EXCHANGES; i++) {
            while (isEmpty(shm)) {
                // sempre que está vazio nao se pode ler
                printf("O buffer está vazio.\n");
                sleep(1);
            }
            int value_readB;
            // ler do buffer
            readB(shm, &value_readB);
        }

        exit(0);
    } else if (pid > 0) {

        for (i = 0; i < EXCHANGES; i++) {

            while (isFull(shm)) {
                 // sempre que está cheio nao se pode escrever
                printf("O buffer está full.\n");
                sleep(1);
            }

            // escrever no buffer
            writeB(shm, i);
        }
        
        wait(NULL);
        
        // remover memoria partilhada

        if (munmap(shm, 150) < 0) {
            exit(1);
        }
        
        if (shm_unlink("/shm_ex14") < 0) {
            exit(1);
        }
    }
	
	return 0;
}

int isEmpty(buffer *shm) {
    if (shm->head == shm->tail) {
        return 1;
    }
    return 0;
}

int isFull(buffer *shm) {
    if ( (shm->head + 1) % shm->size == shm->tail) {
        return 1;
    }
    return 0;
}

void readB(buffer *shm, int *value) {
    // Ler o valor no buffer
    *value = shm->buffer[shm->tail];

    printf("Posição do Buffer: %d\nValor Lido: %d\n\n", shm->tail, *value);
    
    // Avançar com a TAIL do buffer uma posição
    shm->tail = (shm->tail + 1) % shm->size;

}

void writeB(buffer *shm, int value) {
    // Escrever no buffer
    shm->buffer[shm->head] = value;

	printf("Posição do Buffer: %d\nValor Escrito: %d\n\n", shm->head, value);

    // Avançar com a Head do buffer uma posição
    shm->head = (shm->head + 1) % shm->size;
}
