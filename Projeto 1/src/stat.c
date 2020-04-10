

#include "stat.h"
#include "signal_handlers.h"

static int block_size; 
pid_t child = 0;
pid_t main_prg;
bool isDirectory(char * pathname)
{
    struct stat info;
    if(stat(pathname, &info)<0)
    {
        perror("Error\n");
        logExit(1);
    }
    return S_ISDIR(info.st_mode);
}

bool isLinkOrFile(char * pathname,Args arg){
    struct stat info;
    if(arg.dereference){
        if(stat(pathname, &info)<0)
        {
            perror("Error\n");
            logExit(1);
        }
    }
    else{
    if(lstat(pathname, &info)<0)
        {
            perror("Error\n");
            logExit(1);
        }
    }
    return S_ISLNK(info.st_mode)||S_ISREG(info.st_mode);
}


int callRightFunction(char * pathname, Args arg)
{
    int size;
    struct stat info;
    if (arg.dereference){
        
        if(stat(pathname, &info)<0)
        {
            printf("Error\n");
            logExit(1);
        }
        size =info.st_blocks * 512 /block_size;
        if((info.st_blocks * 512) %block_size)
			size++;
	}
    else{
        
        if(lstat(pathname, &info)<0)
        {
            printf("Error\n");
            logExit(1);
        }
 
        if(!arg.bytes){
            size = info.st_blocks * 512 /block_size;
            if(!S_ISLNK(info.st_mode) && info.st_blocks * 512 %block_size)
			   size++;
        }
        else
            size=info.st_size;
                
        
       

        
    }
 
    return size;
}





int  getDirectoryInfo(char * pathname, int max_depth, Args arg)
{
    sleep(1);
    int sum=0;

    pid_t pid;

    DIR * newDir = opendir(pathname); //apontador para os conteudos da pasta
    struct dirent *dp;
       
    char **new_files = malloc(400*sizeof(char*));
        
           
    int idx = 0;
    while((dp = readdir(newDir)) != NULL)
    {
        if ( !strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") )
            continue;
        else 
        {
            new_files[idx] = malloc(400*sizeof(char));
        
            strcpy(new_files[idx], dp->d_name);
            idx++;
                
        }
    }
    closedir(newDir);
    
    char **new_paths = malloc(idx*sizeof(char*));
    for (int i = 0; i < idx; i++)
    {
        new_paths[i] = malloc(400*sizeof(char));
    }
    char *newPathname=malloc(strlen(pathname)*sizeof(char)); 
    strcpy(newPathname,pathname);
     
    newPathname = pathname;
    strcat(newPathname,"/");
    char *tmp_path = (char*) malloc(400*sizeof(char));
    for (int i = 0; i < idx; i++)
    {

        strcpy(tmp_path, newPathname);
        strcat(tmp_path, new_files[i]);
        strcpy(new_paths[i], tmp_path);
    
    }

    for(int i = 0; i < idx; i++)
    {

        if(isLinkOrFile(new_paths[i],arg)){

            int s=callRightFunction(new_paths[i], arg);
            sum+=s;
            if(max_depth>0 && arg.all)
                printResult(s,new_paths[i]);
        }
        else if(isDirectory(new_paths[i]))
        {
            int fd[2];

            if (pipe(fd) < 0) {
                perror("Pipe error!");
                logExit(1);
            }
            pid = fork();
            if (pid < 0)
            {
                perror("Error\n");
                logExit(1);
            }
            else if (pid > 0) //processo pai
            {
                if (getpgrp() == main_prg) { child = pid; }
				close(fd[WRITE]);

				int status = 0;
                waitpid(-1, &status, 0);
 
                int s;         
                if(!arg.sep_dirs){
                    read(fd[READ],&s,sizeof(int));
                    sum+=s;
                    char temp[30];
                    sprintf(temp, "%d", s);
                    logRecvPipe(temp);
                }

            }
            else //processo filho
            {
				if (getpgrp() == main_prg) {
                    setpgid(getpid(), getpid());
                }


				close(fd[READ]);
				int size = getDirectoryInfo(new_paths[i], max_depth-1,arg);
				logCreateFork(arg, max_depth - 1, new_paths[i]);
				if (!arg.sep_dirs)
				{
					write(fd[WRITE],&size,sizeof(size));
                    char temp[30];
                    sprintf(temp, "%d", size);
                    logSendPipe(temp);
				}
				logExit(0);
                    
            }
        }
         
    }


    for (int i=0; i<idx;i++){
        free(new_files[i]);
        free(new_paths[i]);
    }
    free(new_files);
    free(new_paths);


    sum+=callRightFunction(pathname,arg);
    if(max_depth>=0)
        printResult(sum,pathname);
    
    return sum;

}

int setBlockSize(int size)
{
    block_size = size;
    return 0;
}
