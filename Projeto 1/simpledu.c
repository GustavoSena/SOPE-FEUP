#include <stdio.h> 
#include <signal.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
//#include "simpledu.h"

#define BUFFER_SIZE 512 

void sigint_handler(int signo)
{
    write(STDOUT_FILENO, "Are you sure you want to exit?\n ", 33);
}

void sigcont_handler(int signo)
{
    write(STDOUT_FILENO, "Continuing processes...\n", 25);
}

void sigterm_handler(int signo)
{
    write(STDOUT_FILENO, "Terminating processes...\n", 26);
}

int main(int argc, char *argv[], , char *envp[])
{
    char[BUFFER_SIZE] command;
    
    struct sigaction act_int;  
    act_int.sa_handler = sigint_handler;  
    sigemptyset(&act_int.sa_mask);  
    act_int.sa_flags = 0;  

    struct sigaction act_term;
    act_term.sa_handler = sigterm_handler;
    sigempty(&act_terms.sa_mask);
    act_term.sa_flags = 0;

    struct sigaction act_cont;
    act_cont.sa_handler = sigcont_handler;
    sigemptyset(&act_cont.sa_handler);
    act_cont.sa_flags = 0;

    if (sigaction(SIGINT,&act_int,NULL) < 0)  {        
        fprintf(stderr,"Unable to install SIGINT handler\n");        
        exit(1);  
    } 

    if (sigaction(SIGTERM,&act_term,NULL) < 0)  {        
        fprintf(stderr,"Unable to install SIGTERM handler\n");        
        exit(1);  
    } 

    if (sigaction(SIGINT,&action,NULL) < 0)  {        
        fprintf(stderr,"Unable to install SIGCONT handler\n");        
        exit(1);  
    } 

    int all=0;
    int bytes=0;
    int block_size=0;
    int dereference=0;
    int sep_dirs=0;
    int max_depth=_INT16_MAX_;

    for (int i = 1; i < argc; i++) {

		if (strcmp(argv[i], "-a") == 0 ||strcmp(argv[i], "--all")==0) {
			all=1;
			continue;
		}

		if (strcmp(argv[i], "-b") == 0||strcmp(argv[i], "--bytes")==0) {
			bytes= 1;
			continue;
		}

		if (strcmp(argv[i], "-B") == 0) {
			block_size=argv[i+1];
			continue;
		}

		if (strncmp(argv[i], "--block-size=",13) == 0) {
			char str[]=argv[i];
            char* token = strtok(str, "="); 
            int i;
            token = strtok(NULL, "-"); 
            sscanf(token, "%d",  &block_size);
			continue;
		}

		if (strcmp(argv[i], "-L") == 0||strcmp(argv[i], "--dereference") == 0) {
			dereference=1;
			continue;
		}

		if (strcmp(argv[i], "-S") == 0||strcmp(argv[i], "--separate-dirs") == 0) {
			sep_dirs=1;
			continue;
		}

        if (strncmp(argv[i], "--max-depth=",12) == 0) {
			char str[]=argv[i];
            char* token = strtok(str, "="); 
            int i;
            token = strtok(NULL, "-"); 
            sscanf(token, "%d",  &max_depth);
			continue;
		}
    }
}


