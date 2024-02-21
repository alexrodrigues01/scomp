#include "record.h"

int verificarNumber(int number, record *shm, program *prog, sem_t *sem);

int main()
{
    /*
    / Criar todas os atributos necessarios
    */
    int data_size = size * sizeof(record), fd, fd2, i;
    sem_t *sem;
    record *shm;
    program *shm2;
    if ((sem = sem_open("/sem", 0)) == SEM_FAILED)
    {
        perror("Error in sem()");
        exit(1);
    }
    /*
        Abrir/Criar Array de Records
    */
    fd = shm_open("/shm", O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        printf("Error on attempting to open shared memory 1!\n");
        exit(EXIT_FAILURE);
    }
    int aux = ftruncate(fd, data_size);
    if (aux == -1)
    {
        printf("Error while defining the size of shared memory 12!\n");
        exit(EXIT_FAILURE);
    }
    shm = (record *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    /*
        Abrir/Criar Array de Funcionalidades
    */

    fd2 = shm_open("/shm2", O_RDWR, S_IRUSR | S_IWUSR);
    if (fd2 == -1)
    {
        printf("Error on attempting to open shared memory!\n");
        exit(EXIT_FAILURE);
    }

    int aux2 = ftruncate(fd2, sizeof(program));
    if (aux2 == -1)
    {
        printf("Error while defining the size of shared memory!\n");
        exit(EXIT_FAILURE);
    }
    shm2 = (program *)mmap(NULL, sizeof(program), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);

    /**
     *Fim de Criar Atributos 
     */

    int number;

    printf("===+Inserir Personal Data Record+===\n");
    printf("Number: \n");
    scanf("%d", &number);
    int verficacao = verificarNumber(number, shm, shm2, sem);
    if (verficacao == 1)
    {
        // se tiver um number já existente
        printf("Number existente!!");
    }
    else
    {
        char nome[50], address[50];
        // limpar
        fgets(nome, sizeof(nome), stdin);

        printf("Nome: \n");
        fgets(nome, sizeof(nome), stdin);
        printf("Address: \n");
        fgets(address, sizeof(address), stdin);

        if (sem_wait(sem) == -1)
        {
            perror("Erro no decremento do semáforo");
            exit(1);
        }

        /**
         * Insert
         */

        shm[shm2->nrClientes].number = number;
        strcpy(shm[shm2->nrClientes].name, nome);
        strcpy(shm[shm2->nrClientes].address, address);
        shm2->nrClientes++;
        printf("\nInserting");
        fflush(stdout);
        for (i = 0; i < 10; i++)
        {
            sleep(1);
            printf(".");
            fflush(stdout);
        }
        printf("\n");

        if (sem_post(sem) == -1)
        {
            perror("Erro no decremento do semáforo");
            exit(1);
        }
        /**
         * Insert Fim
         */
    }

    return 0;
}

int verificarNumber(int number, record *shm, program *prog, sem_t *sem)
{
    if (sem_wait(sem) == -1)
    {
        perror("Erro no decremento do semáforo");
        exit(1);
    }
    int i;
    for (i = 0; i < prog->nrClientes; i++)
    {
        if (number == shm[i].number)
        {
            return 1;
        }
    }

    if (sem_post(sem) == -1)
    {
        perror("Erro no decremento do semáforo");
        exit(1);
    }
    return 0;
}
