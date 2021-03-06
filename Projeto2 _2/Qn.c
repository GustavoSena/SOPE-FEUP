
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
#include <signal.h>
#include <semaphore.h>
#include "args.h"
#include "utils.h"
#include "reg.h"
#include "queue.h"


pthread_mutex_t mut=PTHREAD_MUTEX_INITIALIZER;

int pl = 1;
int order = 1;
int limit_wc;
int limit_threads;
struct Queue* queue;
sem_t nthreads;
sem_t nplaces;
int flag = 0;


void sigalarm_handler(int signo){
	flag = 1;
	printf("Alarm ringing\n");

}


void * dealRequest(void * arg) {
    pthread_detach(pthread_self());
   
    int fd;
    Request request = *(Request *) arg;
    char private_fifo[50];
    fifo_name(request.pid, request.tid, private_fifo);
    request.pid = getpid();
    request.tid = pthread_self();

	int n_tries = 0;
	while ((fd = open(private_fifo, O_WRONLY)) < 0)
	{
		
        usleep(1000);
        if(n_tries++>5){
            logGaveUp(request);
            if (limit_threads)
		        sem_post(&nthreads);
            return NULL;
		}

	}


    int place;
    if (limit_wc) {
        sem_wait(&nplaces);
        pthread_mutex_lock(&mut);
        place = dequeue(queue);
        pthread_mutex_unlock(&mut);
    } else {
        pthread_mutex_lock(&mut);
        place = pl;
        pl++;
        pthread_mutex_unlock(&mut);
    }
    request.pl = place;

    n_tries = 0;
    while(write(fd, &request, sizeof(request))<= 0){

		printf("Sending reponse\n");
		if (n_tries++ > 5)
		{
			logGaveUp(request);

            if (limit_threads)
                sem_post(&nthreads);
            if (limit_wc) { 
                pthread_mutex_lock(&mut);
                enqueue(queue, place);
                pthread_mutex_unlock(&mut);
                sem_post(&nplaces); 
                
            }
            close(fd);
			return NULL;
		}
	}
    
   
    logEnter(request);
    usleep(request.dur*1000);
    logTimeUp(request);
       
       
   

    close(fd);
    
    if (limit_threads)
		sem_post(&nthreads);
	if (limit_wc)
	{
		pthread_mutex_lock(&mut);
        enqueue(queue, place);
        pthread_mutex_unlock(&mut);
        sem_post(&nplaces);
	}

	return NULL;
}


void *tooLate(void *arg){
    
   
    int fd;
    Request request = *(Request *) arg;
    char private_fifo[50];
    fifo_name(request.pid, request.tid, private_fifo);
    request.pid = getpid();
    request.tid = pthread_self();
	request.pl = -1;

	int n_tries = 0;
	while ((fd = open(private_fifo, O_WRONLY)) < 0)
	{
		
        usleep(1000);
        if(n_tries++>1){
            logGaveUp(request);
            if (limit_threads)
		        sem_post(&nthreads);
            return NULL;
		}

	}
  

    n_tries = 0;
    while(write(fd, &request, sizeof(request))<=0){
     	
        if (n_tries++ > 1){
			logGaveUp(request);

            if (limit_threads)
                sem_post(&nthreads);
            close(fd);
			return NULL;
		}
    }
    

    close(fd);
    
    log2Late(request);

    if (limit_threads)
		sem_post(&nthreads);


    return NULL;
}



int main(int argc, char *argv[])
{

    struct sigaction act_alarm;
    act_alarm.sa_handler = sigalarm_handler;
    sigemptyset(&act_alarm.sa_mask);
    act_alarm.sa_flags = 0;

    if (sigaction(SIGALRM,&act_alarm,NULL) < 0)  {        
        fprintf(stderr,"Unable to install SIGALARM handler\n");        
        exit(1);  
    }  



    Args_qn arg = process_args_qn(argc, argv);
    char public_fifo[30];
    strcpy(public_fifo, arg.fifoname);
    int fd1;

   
    if(arg.nplaces)
		limit_wc = 1;
    if(arg.nthreads)
	    limit_threads = 1;
	Request request;

    if(mkfifo(public_fifo, 0660)!=0){
        unlink(public_fifo); 
        if(mkfifo(public_fifo, 0660)!=0){
            perror("error creating public fifo\n");
			exit(1);
		}
	}

    fd1 = open(public_fifo, O_RDONLY | O_NONBLOCK);
    
    pthread_t tid;
	

    if (limit_threads) {
        sem_init(&nthreads, 0, arg.nthreads);
    }
    if (limit_wc) {
        sem_init(&nplaces, 0, arg.nplaces);
        queue = createQueue(arg.nplaces);
		fillQueue(queue);
	}

	alarm(arg.nsecs);


	while(1)
	{

        if(flag==1){
            break;
        }
        if(read(fd1, &request, sizeof(request))>0)
        { 
            logRecv(request);
            if (limit_threads)  
                sem_wait(&nthreads);

            pthread_create(&tid, NULL, dealRequest, (void *)&request);
            
        } 
  
     
    } 

  
    
    unlink(public_fifo);
	usleep(5e5);


	while(read(fd1, &request, sizeof(request))>0) //limpar o resto dos pedidos
    {
        if (limit_threads)  
            sem_wait(&nthreads); 

        
        pthread_create(&tid, NULL, tooLate, (void *)&request);
        pthread_join(tid,NULL);
    }
    
   
    
    close(fd1);
    
    pthread_mutex_destroy(&mut);
    pthread_exit(0);
   
}