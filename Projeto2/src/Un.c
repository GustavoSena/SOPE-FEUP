
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
    request.dur = (rand() % 410001) + 10000;


	


	write(fd, &request, sizeof(request));

	char fifo[50];
    fifo_name(request.pid, request.tid, fifo);
    printf("Got private fifo\n");
    mkfifo(fifo, 0660);

    fd2 = open(fifo, O_RDONLY);

	logWant(request);
	close(fd2);
	unlink(fifo);

    return NULL;

}


int main(int argc, char *argv[])
{

    printf("Entered main\n");
    Args_un arg = process_args_un(argc, argv);
    printf("Processed args\n");
    strcpy(public_fifo, arg.fifoname);
    printf("%s\n", public_fifo);
    
    int i = 1;
    int current_time = 0;
    do{
        printf("try to open fifo\n");
        fd = open(public_fifo, O_WRONLY);
    } while(fd == -1);

    printf("opened fifo\n");
    while(current_time < arg.nsecs)
    {
        pthread_t tid;
        pthread_create(&tid, NULL, sendRequest, (void *)&i);
        i++;
        current_time+=2;
        usleep(2*1000000);
    }

    close(fd);

    pthread_exit(0);

}