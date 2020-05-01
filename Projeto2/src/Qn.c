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


char public_fifo[30];
int current_time;
int max_time;
int order;
pthread_mutex_t mut=PTHREAD_MUTEX_INITIALIZER; 

void * dealRequest(void * arg) {

    pthread_mutex_lock(&mut);
    printf("Thread created\n");
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

    
    write(fd, &request, sizeof(request));
    printf("Wrote answer\n");
    close(fd);
    
    pthread_mutex_unlock(&mut);
    return NULL;
}

int main(int argc, char *argv[])
{
    Args_qn arg = process_args_qn(argc, argv);
    max_time = arg.nsecs;
    strcpy(public_fifo, arg.fifoname);
    printf("%s\n", public_fifo);
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
    printf("created fifo\n");
    fd1 = open(public_fifo, O_RDONLY | O_NONBLOCK);
    printf("%d\n", fd1);
    printf("opened fifo\n");
    pthread_t tid;
    do
    {
        if(read(fd1, &request, sizeof(request))>0)
        { 
            
            pthread_create(&tid, NULL, dealRequest, (void *)&request);
            pthread_join(tid, NULL); //acho q n devemos fazer isto
            printf("%d\n", current_time);
            printf("%d\n", max_time);
        } 
   
        
    } while (current_time < max_time);

    printf("Out of first cycle\n");

    sleep(5);

    while(read(fd1, &request, sizeof(request))>0) //limpar o resto dos pedidos
    {
       
        printf("In second cycle\n");
        pthread_create(&tid, NULL, dealRequest, (void *)&request);
        pthread_join(tid,NULL);
    }
    

    
    close(fd1);
    unlink(public_fifo);

    printf("finish program\n");
    pthread_exit(0);
   
}