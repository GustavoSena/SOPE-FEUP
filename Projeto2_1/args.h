#pragma once


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

typedef struct{
    int nsecs;
    int nplaces;
    int nthreads;
    char fifoname[528];
}Args_qn;

typedef struct{
    int nsecs;
    char fifoname[528];
}Args_un;


Args_qn process_args_qn(int argc, char *argv[]);

Args_un process_args_un(int argc, char*argv[]);


void print_usage_qn();

void print_usage_un();