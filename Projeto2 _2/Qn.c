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

int pl = 1; //to figure out



int order;
int limit_wc;
int limit_threads;
struct Queue* queue;
sem_t nthreads;
sem_t nplaces;
static int flag = 0;




void sigalarm_handler(int signo){
	flag = 1;
	printf("Alarm ringing\n");

}

void * dealRequest(void * arg) {

   
    int fd;
    Request request = *(Request *) arg;
    logRecv(request);
    char private_fifo[50];
    fifo_name(request.pid, request.tid, private_fifo);
    request.pid = getpid();
    request.tid = pthread_self();

    pthread_mutex_lock(&mut);

    if (flag!=1) //aceitou o pedido
    {
     
        order++;
        request.pl = order;
        logEnter(request);
        usleep(request.dur*1000);
        logTimeUp(request);
       
       
    }
    else
    {
        log2Late(request);
    }
    
    pthread_mutex_unlock(&mut);
    int tries = 0;
    
    do
    {
        fd = open(private_fifo, O_WRONLY);
        tries++;
        if (tries == 20){
            printf("Couldn't open private fifo\n");
            exit(1);
		}
            
        
    } while (fd == -1);
    if (limit_threads) { sem_post(&nthreads); }

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


    

    int error = 0;
    int n_tries = 0;
    do{
        error = write(fd, &request, sizeof(request));
        n_tries++;
        if (n_tries == 5){
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
    }while(error <= 0);

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
    order = 0;
   
    if(arg.nplaces)
		limit_wc = 1;
    if(arg.nthreads)
	    limit_threads = 1;
	Request request;

	int error;
    do{
        error = mkfifo(public_fifo, 0660);
        if (error < 0)
            unlink(public_fifo);

    }while(error<0);

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

	alarm(arg.nsecs/1000);


    bool read_smt = false;
    int n_tries = 0;
	do
	{
        
        if(read(fd1, &request, sizeof(request))>0)
        { 
            if (limit_threads)  
                sem_wait(&nthreads);
            read_smt = true;
            pthread_create(&tid, NULL, dealRequest, (void *)&request);
            //pthread_join(tid, NULL); 
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
  
     
    } while (flag!=1);

  
    
    unlink(public_fifo); //foi mudado de posição



    while(read(fd1, &request, sizeof(request))>0) //limpar o resto dos pedidos
    {
        if (limit_threads)  
            sem_wait(&nthreads);

        pthread_create(&tid, NULL, dealRequest, (void *)&request);
        //pthread_join(tid,NULL);
    }
    
   
    
    close(fd1);
    
    pthread_mutex_destroy(&mut);
    pthread_exit(0);
   
}