#include "args.h"

void print_usage(){
	printf("Usage: simpledu -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n");
}
Args process_args(int argc, char *argv[]){
    Args args;
    args.all=0;
    args.bytes=0;
    args.block_size=0;
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
		else if (strcmp(argv[i], "-L") == 0||strcmp(argv[i], "--dereference") == 0) {
			args.dereference=1;
			
			
		}
        
		else if (strcmp(argv[i], "-l") == 0) { continue; }
		
		else if (strcmp(argv[i], "-B") == 0) {
			if(i<argc-1){
				i++;
				int size=atoi(argv[i]);
				if (size < 1) { 
					perror("Invalid Block Size!");
					print_usage();
					exit(1);
				} else args.block_size=size;
			}
			else{
				perror("Invalid arguments!\n");
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
			perror("Invalid path!\n");
			exit(1);
			}
			else strcpy(args.path,argv[i]);
				
		} 
		else{
			perror("Invalid arguments!\n");
			print_usage();
			exit(1);
		}

		
    }

	
	return args; 

}


char** get_cmd_args(Args args){
	
    char ** cmd_args = malloc(7 * sizeof(char*));
    for (int i =0 ; i < 7; ++i)
        cmd_args[i] = malloc(30* sizeof(char));
    
    int i=1;
	if(args.all==1){
		strcpy(cmd_args[i],"-a");
		i++;
	}
	if(args.bytes==1){
		strcpy(cmd_args[i],"-b");
		i++;
	}
	if(args.block_size!=0){
		sprintf(cmd_args[i],"--block-size=%d",args.block_size);
		i++;
	}
	if(args.dereference==1){
		strcpy(cmd_args[i],"-L");
		i++;
	}
	if(args.sep_dirs==1){
		strcpy(cmd_args[i],"-S");
		i++;
	}
	if(args.max_depth!=__INT_MAX__){
		sprintf(cmd_args[i],"--max_depth=%d",args.max_depth - 1);
		i++;
	}
	return cmd_args;
}


char * argline(Args args){
	char * line = malloc(1024* sizeof(char));
	strcpy(line,"");
	if(args.all==1){
		strcat(line," -a");
		
	}
	if(args.bytes==1){
		strcat(line," -b");
		
	}
	if(args.block_size!=0){
		char s[40];
		sprintf(s," --block-size=%d",args.block_size);
		strcat(line,s);
		
	}
	if(args.dereference==1){
		strcat(line," -L");
		
	}
	if(args.sep_dirs==1){
		strcat(line," -S");
		
	}
	if(args.max_depth!=__INT_MAX__){
		char s[40];
		sprintf(s," --max_depth=%d",args.max_depth - 1);
		strcat(line,s);
	}
	return line;
}