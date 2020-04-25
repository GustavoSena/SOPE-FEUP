#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <pthread.h>


typedef struct {

    int i;
    int pid;
    long tid;
    int dur; //milissegundos
    int pl;

}Request;


void fifo_name(int pid,long tid, char * f_name);