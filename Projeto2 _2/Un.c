
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

#define WAIT_TIME 50000

int fd;

static int flagUn = 0;




void sigalrm_handler(int signo){
	flagUn = 2;
	printf("Alarm ringing\n");

}

void * sendRequest(void * arg) // arg vai ser número sequencial do pedido
{
	pthread_detach(pthread_self());

	int fd2;
    time_t t;
    srand((unsigned) time(&t));
    Request request;
    request.pid = getpid();
    request.i = *(int *) arg;
    request.tid = pthread_self();
    request.pl = -1;
    request.dur = (rand() % 201) + 50;
    
   
	char fifo[50];
    fifo_name(request.pid, request.tid, fifo);
    if(mkfifo(fifo, 0660)!=0){
        printf("error creating private fifo\n");
        logFailed(request);
        return NULL;
	}

	logWant(request);
    if(write(fd, &request, sizeof(request))<0){
        printf("Error writing request\n"); 
        logFailed(request);
        return NULL;
    }
    

    Request answer;
    fd2 = open(fifo, O_RDONLY);// | O_NONBLOCK);
    if(fd2 < 0){
        printf("Error opening fifo\n");
        logFailed(request);
        return NULL;
    }

    int nread;

    if((nread=read(fd2, &answer, sizeof(answer))<0)){
        printf("Error reading answer\n");
        logFailed(request);
        return NULL;
    }
 
    if(answer.pl != -1)
    {
        logIamIn(answer); 
    }
    else
    {
        logClosed(answer);
    }

    if(unlink(fifo)!=0){
        printf("error unlink\n");
    }

    //printf("After unlink\n");
    if(close(fd2)!=0){
        printf("error fd\n");
    }
    //printf("After close\n");

    return NULL;
}

int *pi = NULL;

int main(int argc, char *argv[])
{

    setbuf(stdout, NULL);


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
    if((fd= open(public_fifo, O_WRONLY))<0){
        printf("Fifo name doesn't match with the server fifo name\n");
        exit(1);
    }


    if(alarm(arg.nsecs)<0){
        printf("Alarm went wrong\n");
		exit(1);
	}

	int i = 0;
    while(1)
    {
        if(flagUn==2){
            break;
        }
        pthread_t tid;
        pi = (int *) malloc (sizeof (int));
        *pi = i;
		//i++;

		while(pthread_create(&tid, NULL, sendRequest, (void *)pi) != 0){
            printf("Error creating thread\n");
            usleep(1000);
        }
        i++;
        usleep(WAIT_TIME); 
    }

    printf("Exited cycle\n");

    close(fd);

    
    pthread_exit(0);

}






/*
static int flagUn = 0;


char public_fifo[20];

void sigalrm_handler(int signo){
	flagUn = 2;
	printf("Alarm ringing\n");

}

void * sendRequest(void * arg) // arg vai ser número sequencial do pedido
{
	pthread_detach(pthread_self());
    int fd;
    int fd2;
    time_t t;
    srand((unsigned) time(&t));
    Request request;
    request.pid = getpid();
    request.i = *(int *) arg;
    request.tid = pthread_self();
    request.pl = -1;
    request.dur = (rand() % 201) + 50;
    

    if((fd= open(public_fifo, O_WRONLY))<0){
        printf("Fifo name doesn't match with the server fifo name\n");
        logFailed(request);
        return NULL;
	}

	
   
	char fifo[50];
    fifo_name(request.pid, request.tid, fifo);
    if(mkfifo(fifo, 0660)!=0){
        printf("error creating private fifo\n");
        logFailed(request);
        return NULL;
	}

	if (write(fd, &request, sizeof(request)) < 0)
	{
		printf("Error writing request\n"); 
        logFailed(request);
        return NULL;
	}
	else
    {

		logWant(request);
	}


    close(fd);


    Request answer;
    fd2 = open(fifo, O_RDONLY);// | O_NONBLOCK);
    if(fd2 < 0){
        printf("Error opening fifo\n");
        logFailed(request);
        return NULL;
    }

    int nread;

    if((nread=read(fd2, &answer, sizeof(answer))<0)){
        printf("Error reading answer\n");
        logFailed(request);
        return NULL;
    }
 
    if(answer.pl != -1)
    {
        logIamIn(answer); 
    }
    else
    {
        logClosed(answer);
    }

    if(unlink(fifo)!=0){
        printf("error unlink\n");
    }

    //printf("After unlink\n");
    if(close(fd2)!=0){
        printf("error fd\n");
    }
    //printf("After close\n");

    



    return NULL;
}

int *pi = NULL;

int main(int argc, char *argv[])
{

    setbuf(stdout, NULL);


    struct sigaction act_alarm;
    act_alarm.sa_handler = sigalrm_handler;
    sigemptyset(&act_alarm.sa_mask);
    act_alarm.sa_flags = 0;

    if (sigaction(SIGALRM,&act_alarm,NULL) < 0)  {        
        fprintf(stderr,"Unable to install SIGALARM handler\n");        
        exit(1);  
    }  


    Args_un arg = process_args_un(argc, argv);
    
    strcpy(public_fifo, arg.fifoname);
    


    if(alarm(arg.nsecs)<0){
        printf("Alarm went wrong\n");
		exit(1);
	}

	int i = 0;
    while(1)
    {
        if(flagUn==2){
            break;
        }
        pthread_t tid;
        pi = (int *) malloc (sizeof (int));
        *pi = i;
		//i++;

		while(pthread_create(&tid, NULL, sendRequest, (void *)pi) != 0){
            printf("Error creating thread\n");
            usleep(1000);
        }
        i++;
        usleep(WAIT_TIME); 
    }

    printf("Exited cycle\n");

    

    
    exit(0);

}*/