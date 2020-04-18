#include "utils.h"

char * fifo_name(int pid, int tid)
{
    char * str_pid;
    char * str_tid;
    char fifo[50] = "/tmp/";
    sprintf(str_pid, "%d", pid);
    sprintf(str_tid, "%d", tid);
    strcat(fifo, str_pid);
    strcat(fifo, ".");
    strcat(fifo,str_tid);
    
    return fifo;

}