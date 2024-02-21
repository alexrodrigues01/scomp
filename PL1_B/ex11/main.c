#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

int ARRAY_SIZE = 5;

typedef struct
{
    char cmd[32];
    int tempo;
} comando;

int main(void)
{
    comando *array = malloc(36 * ARRAY_SIZE);

    int i, ret, a;

    for (i = 0; i < ARRAY_SIZE; i++)
    {
        char name[80];
        sprintf(name, "Program_%d", i + 1);
        strcpy((array + i)->cmd, name);
        (array + i)->tempo = i + 1;
        if(printf("%s\n", (array + i)->cmd)<0){
            perror("Printf error");
        }
    }

   
    for (a = 0; a < ARRAY_SIZE; a++)
    {
        int exec_pid = fork();

        if(exec_pid==-1){
            perror("Fork error");
        }
        // pai
        if (exec_pid > 0)
        {
            int killer_pid = fork();

            if(killer_pid==-1){
                perror("Fork error");
            }
            // pai
            if (killer_pid > 0)
            {
                waitpid(exec_pid, NULL, 0);
                if(kill(killer_pid, SIGKILL)==-1){
                    perror("Kill error");
                }         
            } else {
				// 2 filho
                for (i =  1; i <= (array+a)->tempo; i++)
                {
                    sleep(1);
                    if(printf("%ds to kill\n",i)<0){
                        perror("Printf error");
                    }
                }

                if(kill(exec_pid, SIGKILL)==-1){
                    perror("Kill error");
                }
                if(printf("The command %s didn't end in its allowed time!\n", (array+a)->cmd)<0){
                    perror("Printf error");  
                }   
				exit(0);
            }
        }
        // primeiro filho que executa o programa
        else
        {
           // ret = execlp("program.c",(array+a)->cmd , (array+a)->tempo, (char *)NULL);
          
          if(printf("\n\nName: %s\n",(array+a)->cmd)<0){
              perror("Printf error");
          }
			sleep((array+a)->tempo);
			if(printf("Execution ended!\n\n")<0){
                perror("Printf error");
            }
           //exit(ret);
           exit(0);
        }

    }
    return 0;
}
