#include "args.h"

void print_usage_qn(){
	printf("Usage: Qn <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n");
}



Args_qn process_args_qn(int argc, char *argv[]){
    Args_qn args;
    args.nsecs=0;
    args.nplaces=__INT_MAX__;
    args.nthreads=__INT_MAX__;
	strcpy(args.fifoname,"");
	
	
    for (int i = 1; i < argc; i++) {
       
		
		if (strcmp(argv[i], "-t") == 0) {
			if(i<argc-1){
				i++;
				int size=atoi(argv[i]);
				if (size < 1) { 
					perror("Invalid Seconds!");
					print_usage_qn();
					exit(1);
				} else args.nsecs=size;
			}
			else{
				perror("Invalid arguments!\n");
				print_usage_qn();
				exit(1);
			}
		}
		
	 	else if (strcmp(argv[i], "-l") == 0) {
			if(i<argc-1){
				i++;
				int size=atoi(argv[i]);
				if (size < 1) { 
					perror("Invalid Places!");
					print_usage_qn();
					exit(1);
				} else args.nplaces=size;
			}
			else{
				perror("Invalid arguments!\n");
				print_usage_qn();
				exit(1);
			}
		}
		

		else if (strcmp(argv[i], "-n") == 0) {
			if(i<argc-1){
				i++;
				int size=atoi(argv[i]);
				if (size < 1) { 
					perror("Invalid Threads!");
					print_usage_qn();
					exit(1);
				} else args.nthreads=size;
			}
			else{
				perror("Invalid arguments!\n");
				print_usage_qn();
				exit(1);
			}
		}
	    else if(strcmp(args.fifoname, "")==0){
			
			 strcpy(args.fifoname,argv[i]);
				
		} 
		else{
			perror("Invalid arguments!\n");
			print_usage_qn();
			exit(1);
		}
		
		
    }
	if (args.nsecs==0){
		perror("Invalid arguments!\n");
		print_usage_qn();
		exit(1);
	}
	
	
	return args; 

}



void print_usage_un(){
	printf("Usage: Un <-t nsecs> fifoname\n");
}


Args_un process_args_un(int argc, char *argv[]){
    Args_un args;
    args.nsecs=0;
	strcpy(args.fifoname,"");
	
	
    for (int i = 1; i < argc; i++) {
       
		
		if (strcmp(argv[i], "-t") == 0) {
			if(i<argc-1){
				i++;
				int size=atoi(argv[i]);
				if (size < 1) { 
					perror("Invalid Seconds!");
					print_usage_un();
					exit(1);
				} else args.nsecs=size;
			}
			else{
				perror("Invalid arguments!\n");
				print_usage_un();
				exit(1);
			}
		}

		
	    else if(strcmp(args.fifoname, "")==0){
			
			 strcpy(args.fifoname,argv[i]);
				
		} 
		else{
			perror("Invalid arguments!\n");
			print_usage_un();
			exit(1);
		}
		
		
    }
	if (args.nsecs==0){
		perror("Invalid arguments!\n");
		print_usage_un();
		exit(1);
	}
	
	
	return args; 

}




