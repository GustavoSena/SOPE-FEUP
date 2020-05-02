#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include "args.h"
#include "utils.h"
#include "reg.h"




int current_time;
int max_time;
int order;


void * dealRequest(void * arg) {

   
    int fd;
    Request request = *(Request *) arg;
    logRecv(request);
    char private_fifo[50];
    fifo_name(request.pid, request.tid, private_fifo);
    request.pid = getpid();
    request.tid = pthread_self();

    if (current_time < max_time) //aceitou o pedido
    {
        
        order++;
        request.pl = order;
        current_time += request.dur/1000;
        logEnter(request);
        sleep(request.dur/1000);
        logTimeUp(request);
    }
    else
    {
        log2Late(request);
    }
    

    
    do
    {
        fd = open(private_fifo, O_WRONLY);

    } while (fd == -1);

    int error = 0;
    int n_tries = 0;
    do{
        error = write(fd, &request, sizeof(request));
        n_tries++;
        if (n_tries == 50)
            break;

    }while(error <= 0);

    if(n_tries == 50)
        logGaveUp(request);
    
    close(fd);
    

    return NULL;
}

int main(int argc, char *argv[])
{
    Args_qn arg = process_args_qn(argc, argv);
    max_time = arg.nsecs;
    char public_fifo[30];
    strcpy(public_fifo, arg.fifoname);
    int fd1;
    order = 0;
    current_time = 0;
    
    
    Request request;
    int error;
    do{
        error = mkfifo(public_fifo, 0660);
        if (error < 0)
            unlink(public_fifo);
    }while(error<0);
    fd1 = open(public_fifo, O_RDONLY | O_NONBLOCK);
    bool read_smt = false;
    int n_tries = 0;
    pthread_t tid;
    do
    {
        if(read(fd1, &request, sizeof(request))>0)
        { 
            
            read_smt = true;
            pthread_create(&tid, NULL, dealRequest, (void *)&request);
            pthread_join(tid, NULL); 
            
        } 
        n_tries++;
        if(!read_smt)
            sleep(1);
        if(n_tries == 20 && !read_smt)
        {
            perror("Client fifo name doesn't match with the server fifo name\n");
            unlink(public_fifo);
            exit(1);
        }
   
        
    } while (current_time < max_time);

    

    while(read(fd1, &request, sizeof(request))>0) //limpar o resto dos pedidos
    {
       
       
        pthread_create(&tid, NULL, dealRequest, (void *)&request);
        pthread_join(tid,NULL);
    }
    

    
    close(fd1);
    unlink(public_fifo);

    
    pthread_exit(0);
   
}