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

char public_fifo[20];
int current_time;
int max_time;
int order;

void * dealRequest(void * arg) {
    int fd;
    Request request = *(Request *) arg;
    char * private_fifo = fifo_name(request.pid, request.tid);
    request.pid = getpid();
    request.tid = pthread_self();
    if (current_time < max_time)
    {
        request.pl = order++;
        current_time += request.dur;
        usleep(request.dur * 1000);
    }

    do
    {
        fd = open(private_fifo, O_WRONLY);
    } while (fd == -1);

    write(fd, &request, sizeof(request));
    close(fd);
    
    return NULL;
}

int main(int argc, char *argv[])
{
    Args_qn arg = process_args(argc, argv);
    max_time = arg.nsecs;
    strcpy(public_fifo, arg.fifoname);
    int fd1;
    order = 0;
    current_time = 0;
    
    
    Request request;
    mkfifo(public_fifo, 0660);
    fd1 = open(public_fifo, 0660);
    while (read(fd1, &request, sizeof(request))>0)
    {
        pthread_t tid;
        pthread_create(&tid, NULL, dealRequest, (void *)&request);
        //pthread_join(tid, NULL); //acho q n devemos fazer isto
    } 

    close(fd1);
    unlink(public_fifo);

    pthread_exit(0);
   
}