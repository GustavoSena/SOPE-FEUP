

#include "simpledu.h"




/* void sigint_handler(int signo)
{
    write(STDOUT_FILENO, "Are you sure you want to exit?\n ", 33);
}

// void sigcont_handler(int signo)
// {
//     write(STDOUT_FILENO, "Continuing processes...\n", 25);
// }


// void sigterm_handler(int signo)
// {
//     write(STDOUT_FILENO, "Terminating processes...\n", 26);
// }

*/


int main(int argc, char *argv[], char *envp[])
{
/*     char[BUFFER_SIZE] command;
    
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
    }  */

    
    Args args = process_args(argc,argv);
    setBlockSize(args.block_size);
    int total; 
    initLogs();
    
logCreate(argc,argv);
    if(isDirectory(args.path)){
        total = getDirectoryInfo(args.path, args.max_depth, args);
    }
    else {
        total=callRightFunction(args.path, args);
        printResult(total,args.path);
    }
    logExit(0);
    
    return 0;

}
