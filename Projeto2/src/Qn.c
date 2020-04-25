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
int order;
int current_time;
int max_time;

void * dealRequest(void * arg) //o argumento vai ser o descritor do fifo publico
{
    int fd1, fd2;
    Request request;
    fd1 = *(int *) arg;
    if(read(fd1, &request, sizeof(request)) < 0)
        perror("Error\n");
    char * private_fifo = fifo_name(request.pid, request.tid);
    request.pid = getpid();
    request.tid = pthread_self();
    if(current_time < max_time)
    {
        request.pl = order++;
        //fazer o sleep com a dur do request
    }

    do
    {
        fd2 = open(private_fifo, O_WRONLY);
    } while (fd2 == -1);

    write(fd2, &request, sizeof(request));
    close(fd2);
    
    return NULL;
}

int main(int argc, char *argv[])
{
    int fd1;
    order = 0;
    current_time = 0;
    mkfifo(public_fifo, 0660);
    fd1 = open(public_fifo, 0660);
    Args_qn args=process_args_qn(argc,argv);
    //não esquecer de fazer close do fd1 no final e unlink do public_fifo
}