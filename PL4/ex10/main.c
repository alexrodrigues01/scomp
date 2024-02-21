#include "record.h"

//método para correr os diferentes programas em diferentes processos
void run(char *file)
{

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Erro no fork");
        exit(1);
    }
    if (pid == 0)
    {
        execlp(file, file, (char *)NULL);
        exit(0);
    }
    while (wait(NULL) > 0)
    {
    }
}

int main()
{
    sem_unlink("/sem");
    shm_unlink("/shm");
    shm_unlink("/shm2");
    int data_size = size * sizeof(record), fd, fd2, option;
    record *shm;
    program *shm2;
    fd = shm_open("/shm", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        printf("Error on attempting to open shared memory!\n");
        exit(EXIT_FAILURE);
    }
    int aux = ftruncate(fd, data_size);
    if (aux == -1)
    {
        printf("Error while defining the size of shared memory!\n");
        exit(EXIT_FAILURE);
    }
    shm = (record *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    fd2 = shm_open("/shm2", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
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

    shm2->nrClientes = 0;

    option = 5;

    sem_t *sem;
    if ((sem = sem_open("/sem", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem()");
        exit(1);
    }

    while (option != 0)
    {
        printf("1- Insert\n");
        printf("2-Consult\n");
        printf("3- Consult all\n");
        printf("0- Sair\n");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            run("./insert");
            break;
        case 2:
            run("./consult");
            break;
        case 3:
            run("./consult_All");
            break;
        case 0:
            return 0;
        default:
            scanf("%d", &option);
        }
    }

    if (munmap((void *)shm, sizeof(record)) < 0)
    {
        perror("Error at munmap()!\n");
        exit(0);
    }

    if (munmap((void *)shm2, sizeof(program)) < 0)
    {
        perror("Error at munmap()!\n");
        exit(0);
    }

    if (close(fd) < 0)
    {
        perror("Erro a fechar!\n");
        exit(0);
    }

    if (close(fd2) < 0)
    {
        perror("Erro a fechar!\n");
        exit(0);
    }

    return 0;
}