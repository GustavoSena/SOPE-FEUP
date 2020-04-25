#include "utils.h"

void fifo_name(int pid, long tid, char * f_name)
{
    char  str_pid[10];
    char  str_tid[10];
    char fifo[50] = "/tmp/";
    sprintf(str_pid, "%d", pid);
    sprintf(str_tid, "%ld", tid);
    strcat(fifo, str_pid);
    strcat(fifo, ".");
    strcat(fifo,str_tid);
    strcpy(f_name, fifo);
    

}