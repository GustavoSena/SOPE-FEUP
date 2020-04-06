#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

typedef struct{
    int all;
    int bytes;
    int block_size;
    int sep_dirs;
    int dereference;
    int max_depth;
    char path[528];
}Args;

Args process_args(int argc, char *argv[]);



void print_usage();