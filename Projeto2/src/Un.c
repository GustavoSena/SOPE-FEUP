
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

void * sendRequest(void * arg) // arg vai ser número sequencial do pedido
{
    int fd1, fd2;
    time_t t;
    srand((unsigned) time(&t));
    Request request;
    request.pid = getpid();
    request.i = *(int *) arg;
    request.tid = pthread_self();
    request.pl = -1;
    request.dur = (rand() % 410001) + 10000; 

    do{
        fd1 = open(public_fifo, O_WRONLY);
    } while(fd1 == -1);

    write(fd1, &request, sizeof(request));

    char *fifo = fifo_name(request.pid, request.tid);
    mkfifo(fifo, 0660);

    fd2 = open(fifo, O_RDONLY);

    //Tratar da parte de dar display da informação

    

}


int main(int argc, char *argv[])
{

    int i = 1;
    Args_un args=process_args_un(argc,argv);
    return 0;

}