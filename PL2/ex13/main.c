#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#define PIECES_NR 1000

int main()
{
    int fd[4][2], i, pieces, size, notification, storageA1, stock;
    pid_t pid = -1;
    if (printf("Factory Simulation\n") < 0)
    {
        perror("Printf Error");
    }
    for (i = 0; i < 4; i++)
    {
        if (pipe(fd[i]) == -1)
        {
            perror("Pipe failed");

            return 1;
        }
    }

    for (i = 0; i < 4; i++)
    {
        if (pid != 0)
        {
            pid = fork();
            if (pid == -1)
            {
                perror("Fork Error");
            }
        }

        // filho
        if (pid == 0)
        {

            if (i == 0)
            {
                size = PIECES_NR;
                pieces = 5;
                notification = 1;
                close(fd[0][0]);
                while (size != 0)
                {
                    if (printf("M1 Cutting Simulation\n") < 0)
                    {
                        perror("Printf Error");
                    }
                    if (write(fd[0][1], &notification, sizeof(notification)) == -1)
                    {
                        perror("Write Notification1");
                    }
                    if (write(fd[0][1], &pieces, sizeof(pieces)) == -1)
                    {
                        perror("Write Error1");
                    }
                    size -= 5;
                }
                close(fd[0][1]);
                exit(0);
            }
            else if (i == 1)
            {
                close(fd[i - 1][1]);
                close(fd[i][0]);
                size = PIECES_NR;
                while (size != 0)
                {
                    if (printf("M2 Fold Simulation\n") < 0)
                    {
                        perror("Printf Error");
                    }
                    if (read(fd[i - 1][0], &notification, sizeof(notification)) == -1)
                    {
                        perror("Read Error1");
                    }
                    if (read(fd[i - 1][0], &pieces, sizeof(pieces)) == -1)
                    {
                        perror("Read Error2");
                    }
                    if (write(fd[i][1], &notification, sizeof(notification)) == -1)
                    {
                        perror("Write Notification3");
                    }
                    if (write(fd[i][1], &pieces, sizeof(pieces)) == -1)
                    {
                        perror("Write Error4");
                    }
                    size -= pieces;
                }
                close(fd[i - 1][0]);
                close(fd[i][1]);
                exit(0);
            }
            else if (i == 2)
            {
                size = PIECES_NR;
                close(fd[i - 1][1]);
                close(fd[i][0]);
                while (size != 0)
                {
                    stock = 0;
                    if (printf("M3 Weld Simulation\n") < 0)
                    {
                        perror("Printf Error5");
                    }
                    while (stock != 10)
                    {

                        if (read(fd[i - 1][0], &notification, sizeof(notification)) == -1)
                        {
                            perror("Read Error6");
                        }
                        if (read(fd[i - 1][0], &pieces, sizeof(pieces)) == -1)
                        {
                            perror("Read Error7");
                        }
                        stock += pieces;
                    }
                    if (write(fd[i][1], &notification, sizeof(notification)) == -1)
                    {
                        perror("Write Notification8");
                    }
                    if (write(fd[i][1], &stock, sizeof(pieces)) == -1)
                    {
                        perror("Write Error9");
                    }
                    size -= pieces;
                }
                close(fd[i - 1][0]);
                close(fd[i][1]);
                exit(0);
            }
            else
            {
                size = 0;
                close(fd[i - 1][1]);
                close(fd[i][0]);

                while (size != PIECES_NR)
                {
                    stock = 0;
                    if (printf("M4 Packs Simulation\n") < 0)
                    {
                        perror("Printf Error");
                    }
                    while (stock != 100)
                    {

                        if (read(fd[i - 1][0], &notification, sizeof(notification)) == -1)
                        {
                            perror("Read Error10");
                        }
                        if (read(fd[i - 1][0], &pieces, sizeof(pieces)) == -1)
                        {
                            perror("Read Error11");
                        }
                        stock += pieces;
                    }
                    if (write(fd[i][1], &notification, sizeof(notification)) == -1)
                    {
                        perror("Write Notification8");
                    }

                    if (write(fd[i][1], &stock, sizeof(pieces)) == -1)
                    {
                        perror("Write Error12");
                    }
                    size += stock;
                }

                close(fd[i - 1][0]);
                close(fd[i][1]);
                exit(0);
            }
        }
    }

    storageA1 = 0;

    close(fd[3][1]);

    while (storageA1 != PIECES_NR)
    {
        if (printf("M4 Packs Simulation\n") < 0)
        {
            perror("Printf Error");
        }
        if (read(fd[3][0], &notification, sizeof(notification)) == -1)
        {
            perror("Read Error10");
        }
        if (read(fd[3][0], &pieces, sizeof(pieces)) == -1)
        {
            perror("Read Error11");
        }
        if (printf("Storage A1 - added %d pieces\n", pieces) < 0)
        {
            perror("Printf Error");
        }
        storageA1 += pieces;
        printf("Storage A1 : %d\n", storageA1);
    }

    if(printf("Final storage A1 : %d\n", storageA1)<0){
        perror("Printf error");
    }
    close(fd[3][0]);
    return 0;
}