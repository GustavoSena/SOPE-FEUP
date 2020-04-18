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

Args_qn process_args(int argc, char *argv[]);



void print_usage();