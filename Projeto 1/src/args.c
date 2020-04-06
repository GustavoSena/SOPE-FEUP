#include "args.h"

void print_usage(){
	printf("Usage: simpledu -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n");
}
Args process_args(int argc, char *argv[]){
    Args args;
    args.all=0;
    args.bytes=0;
    args.block_size=1024;
    args.dereference=0;
    args.sep_dirs=0;
    args.max_depth=__INT_MAX__;
	strcpy(args.path,"./");
	
	
    for (int i = 1; i < argc; i++) {
        
		if (strcmp(argv[i], "-a") == 0 ||strcmp(argv[i], "--all")==0) {
			args.all=1;
			
		}
		
		else if (strcmp(argv[i], "-b") == 0||strcmp(argv[i], "--bytes")==0) {
			args.bytes= 1;
			
		}

		else if (strcmp(argv[i], "-l") == 0) { continue; }
		
		else if (strcmp(argv[i], "-B") == 0) {
			if(i<argc-1){
				i++;
				int size=atoi(argv[i]);
				if (size < 1) { 
					printf("Invalid Block Size!");
					print_usage();
					exit(1);
				} else args.block_size=size;
			}
			else{
				printf("Invalid arguments!\n");
				print_usage();
				exit(1);
			}
		}
		
	 	else if (strncmp(argv[i], "--block-size=",13) == 0) {
			char str[100];
			strcpy(str,argv[i]);
            char* token = strtok(str, "="); 
            token = strtok(NULL, "-"); 
            sscanf(token, "%d",  &args.block_size);
			
		} 
		
		else if (strcmp(argv[i], "-L") == 0||strcmp(argv[i], "--dereference") == 0) {
			args.dereference=1;
			
		}
        
		else if (strcmp(argv[i], "-S") == 0||strcmp(argv[i], "--separate-dirs") == 0) {
			args.sep_dirs=1;
			
		}

        else if (strncmp(argv[i], "--max-depth=",12) == 0) {
			char str[100];
			strcpy(str,argv[i]);
            char* token = strtok(str, "="); 

            token = strtok(NULL, "-"); 
            sscanf(token, "%d",  &args.max_depth);
			
		}
	    else if(strcmp(args.path, "./")==0){
			
			struct stat info;
			if(stat(argv[i], &info)<0){
			printf("Invalid path!\n");
			exit(1);
			}
			else strcpy(args.path,argv[i]);
				
		} 
		else{
			printf("Invalid arguments!\n");
			print_usage();
			exit(1);
		}

		
    }
	return args; 

}


char **get_cmd_args(Args args){
	char **cmd_args;
	
}
