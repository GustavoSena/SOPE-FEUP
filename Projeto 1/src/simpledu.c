
#include "simpledu.h"
#include "signal_handlers.h"
extern pid_t main_prg;

int main(int argc, char *argv[], char *envp[])
{
    

    main_prg = getpgrp();
    struct sigaction act_int;  
    act_int.sa_handler = sigint_handler;  
    sigemptyset(&act_int.sa_mask);  
    act_int.sa_flags = 0;  

    struct sigaction act_term;
    act_term.sa_handler = sigterm_handler;
    sigemptyset(&act_term.sa_mask);
    act_term.sa_flags = 0;

	struct sigaction act_cont;
    act_cont.sa_handler = sigcont_handler;
    sigemptyset(&act_cont.sa_mask);
    act_cont.sa_flags = 0;



    initLogs();
    if (sigaction(SIGINT,&act_int,NULL) < 0)  {        
        fprintf(stderr,"Unable to install SIGINT handler\n");        
        logExit(1);  
    } 

    if (sigaction(SIGTERM,&act_term,NULL) < 0)  {        
        fprintf(stderr,"Unable to install SIGTERM handler\n");        
        logExit(1);  
    } 

    if (sigaction(SIGCONT,&act_cont,NULL) < 0)  {        
        fprintf(stderr,"Unable to install SIGCONT handler\n");        
        logExit(1);  
    }  

	//signal(SIGINT, sigint_handler);

	Args args = process_args(argc,argv);
    setBlockSize(args.block_size);
    int total; 
    
    logCreate(argc,argv);
    if(isDirectory(args.path)){
        total = getDirectoryInfo(args.path, args.max_depth, args);
    }
    else {
        total=callRightFunction(args.path, args);
        printResult(total,args.path);
    }
    logExit(0);
    


}
