
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
	//pthread_detach(pthread_self());

	int fd2;
    time_t t;
    srand((unsigned) time(&t));
    Request request;
    request.pid = getpid();
    request.i = *(int *) arg;
    request.tid = pthread_self();
    request.pl = -1;
    request.dur = (rand() % 11) + 5;
    
   
	char fifo[50];
    fifo_name(request.pid, request.tid, fifo);
    if(mkfifo(fifo, 0660)!=0){
        perror("error creating private fifo");
    }

    logWant(request);
    if(write(fd, &request, sizeof(request))<0)
        printf("Error writing request\n"); 

    Request answer;
    fd2 = open(fifo, O_RDONLY);// | O_NONBLOCK);
    if(fd2 < 0){
        perror("Error opening fifo\n");
        logFailed(request);
    }

    int nread;
    if((nread=read(fd2, &answer, sizeof(answer))<0){
        logFailed(request);
    }
    if(answer.pl != -1) // se o pedido foi aceite
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

    printf("After unlink\n");
    if(close(fd2)!=0){
        printf("error fd\n");
    }
    printf("After close\n");

    //unlink(fifo);
    //close(fd2);
    /*if(error < 0){
        perror("Error creating fifo\n");
    }    
    fd2 = open(fifo, O_RDONLY);// | O_NONBLOCK);
    if(fd2 < 0){
        perror("Error opening fifo\n");
    }

    Request answer;
    

    int error2;
    int n_tries = 0;
    read(fd2, &answer, sizeof(answer));
    do
    {
        error2 = read(fd2, &answer, sizeof(answer));
        n_tries++;
        if(n_tries == 50)
            break;
        usleep(1000000);
        printf("trying again: %d\n",request.i);
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
    return NULL;*/

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
    if(open(public_fifo, O_WRONLY)<0){
        perror("Fifo name doesn't match with the server fifo name");
        exit(1);
    }
    
    int i = 0;
    do{
        
        fd = open(public_fifo, O_WRONLY);
    } while(fd == -1);

    if(alarm(arg.nsecs/1000)<0)
        printf("Alarm went wrong\n");
    
    while(1)
    {
        if(flagUn==2){
            break;
        }
        pthread_t tid;
        pi = (int *) malloc (sizeof (int));
        *pi = i;
        i++;
        while(pthread_create(&tid, NULL, sendRequest, (void *)pi) != 0){
            printf("Error creating thread");
            usleep(1000);
        }
        //i++;
        usleep(WAIT_TIME); //terá que ser na ordem dos milissegundos
       
    }

    printf("Exited cycle\n");

    close(fd);

    
    pthread_exit(0);

}