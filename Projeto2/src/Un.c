
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
    

    int fd2;
    time_t t;
    srand((unsigned) time(&t));
    Request request;
    request.pid = getpid();
    request.i = *(int *) arg;
    request.tid = pthread_self();
    request.pl = -1;
    request.dur = (rand() % 10001) + 5000;
    //request.dur = 5000; //5 segundos para efeitos de teste


	

    
    write(fd, &request, sizeof(request)); 
   

	char fifo[50];
    fifo_name(request.pid, request.tid, fifo);
    int error = mkfifo(fifo, 0660);
    if(error < 0)
        perror("Error creating fifo\n");
    fd2 = open(fifo, O_RDONLY | O_NONBLOCK);
    if(fd2 < 0)
        perror("Error opening fifo\n");
    

    Request answer;
    

    int error2;
    int n_tries = 0;
    do
    {
        error2 = read(fd2, &answer,sizeof(answer));
        n_tries++;
        if(n_tries == 50)
            break;
        sleep(2);
    } while (error2 <= 0);

    

    if(n_tries == 50)
    {
        logFailed(request);
    }
    else
    {
        if(answer.pl != -1) // se o pedido foi aceite
        {
            logIamIn(answer); //Ver se é suposto apresentar a informação do request do cliente ou do servidor
        }
        else
        {
            logClosed(answer);
        }
    }

    close(fd2);
    unlink(fifo);
    return NULL;

}


int main(int argc, char *argv[])
{


    Args_un arg = process_args_un(argc, argv);
    strcpy(public_fifo, arg.fifoname);
    if(open(public_fifo, O_WRONLY)<0){
        perror("Fifo name doesn't match with the server fifo name");
        exit(1);
    }
    
    int i = 0;
    int current_time = 0;
    do{
        
        fd = open(public_fifo, O_WRONLY);
    } while(fd == -1);

    
    while(current_time < arg.nsecs)
    {
        pthread_t tid;
        pthread_create(&tid, NULL, sendRequest, (void *)&i);
        i++;
        current_time+=2;
        usleep(2*1000000); //2 segundos
       
    }

    

    close(fd);

    
    pthread_exit(0);

}