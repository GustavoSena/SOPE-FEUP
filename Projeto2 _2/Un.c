
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include "args.h"
#include "utils.h"
#include "reg.h"

#define WAIT_TIME 10000

int fd;

static int flagUn = 0;




void sigalrm_handler(int signo){
	flagUn = 2;
	printf("Alarm ringing\n");

}

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
    request.dur = (rand() % 11) + 5;
    


	
    logWant(request);
    write(fd, &request, sizeof(request)); 
   

	char fifo[50];
    fifo_name(request.pid, request.tid, fifo);
    int error = mkfifo(fifo, 0660);
    if(error < 0){
        printf("Exiting\n");

        perror("Error creating fifo\n");
    }    
    fd2 = open(fifo, O_RDONLY | O_NONBLOCK);
    if(fd2 < 0){
        printf("Exiting\n");
        perror("Error opening fifo\n");
    }

    Request answer;
    

    int error2;
    int n_tries = 0;
    do
    {
        error2 = read(fd2, &answer,sizeof(answer));
        n_tries++;
        if(n_tries == 50)
            break;
        usleep(2000000);
    } while (error2 <= 0);

    

    if(n_tries == 50)
    {
        logFailed(request);
    }
    else
    {
        if(answer.pl != -1) // se o pedido foi aceite
        {
            logIamIn(answer); 
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


    struct sigaction act_alarm;
    act_alarm.sa_handler = sigalrm_handler;
    sigemptyset(&act_alarm.sa_mask);
    act_alarm.sa_flags = 0;

    if (sigaction(SIGALRM,&act_alarm,NULL) < 0)  {        
        fprintf(stderr,"Unable to install SIGALARM handler\n");        
        exit(1);  
    }  


    Args_un arg = process_args_un(argc, argv);
    char public_fifo[20];
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

    if(alarm(arg.nsecs/1000)<0)
        printf("Alarm went wrong\n");
    
    while(flagUn!=2)
    {
        pthread_t tid;
        pthread_create(&tid, NULL, sendRequest, (void *)&i);
        i++;
        current_time+=10;
        usleep(WAIT_TIME); //terá que ser na ordem dos milissegundos
       
    }

    printf("Exited cycle\n");

    close(fd);

    
    pthread_exit(0);

}