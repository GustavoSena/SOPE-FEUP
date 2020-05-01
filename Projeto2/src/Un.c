
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <pthread.h>
#include "args.h"
#include "utils.h"
#include "reg.h"


char public_fifo[20];
int fd;

void * sendRequest(void * arg) // arg vai ser número sequencial do pedido
{
    printf("Entered thread\n");
    int fd2;
    time_t t;
    srand((unsigned) time(&t));
    Request request;
    request.pid = getpid();
    request.i = *(int *) arg;
    request.tid = pthread_self();
    request.pl = -1;
    //request.dur = (rand() % 410001) + 10000;
    request.dur = 5000; //5 segundos para efeitos de teste


	

    //O programa termina aqui quando o tempo em qn acaba e não percebo porquê
    write(fd, &request, sizeof(request)); 
    printf("%d: sent\n", request.i);

	char fifo[50];
    fifo_name(request.pid, request.tid, fifo);
    int error = mkfifo(fifo, 0660);
    if(error < 0)
        printf("Error creating fifo\n");
    fd2 = open(fifo, O_RDONLY | O_NONBLOCK);
    if(fd2 < 0)
        printf("Error opening fifo\n");

    Request answer;
    

    int error2;
    do
    {
        error2 = read(fd2, &answer,sizeof(answer));
    } while (error2 <= 0);
    
    
    if(answer.pl != -1) // se o pedido foi aceite
    {
        logIamIn(answer); //Ver se é suposto apresentar a informação do request do cliente ou do servidor
    }
    else
    {
        logClosed(answer);
    }
    

    close(fd2);
    unlink(fifo);

    return NULL;

}


int main(int argc, char *argv[])
{


    Args_un arg = process_args_un(argc, argv);
    strcpy(public_fifo, arg.fifoname);

    
    int i = 1;
    int current_time = 0;
    do{
        
        fd = open(public_fifo, O_WRONLY);
    } while(fd == -1);

    
    while(current_time < arg.nsecs)
    {
        pthread_t tid;
        pthread_create(&tid, NULL, sendRequest, (void *)&i);
        //pthread_join(tid, NULL);
        i++;
        current_time+=2;
        usleep(2*1000000); //2 segundos
        printf("Current time: %d\n", current_time);

    }

    printf("Out of while cycle\n");

    close(fd);

    printf("finish program\n");
    pthread_exit(0);

}