#include "stat.h"


static int block_size; 

bool isDirectory(char * pathname)
{
    struct stat info;
    if(lstat(pathname, &info)<0)
    {
        perror("Error\n");
        logExit(1);
    }
    return S_ISDIR(info.st_mode);
}

int callRightFunction(char * pathname, Args arg)
{


    if (arg.dereference){
        struct stat info;
        if(stat(pathname, &info)<0)
        {
            printf("Error\n");
            logExit(1);
        }
        int size = info.st_size/block_size;
        if (info.st_size%block_size != 0)
            size++;
        
        printResult(size,pathname);
        return size;
    }
    else{
        struct stat info;
        if(lstat(pathname, &info)<0)
        {
            printf("Error\n");
            logExit(1);
        }
        
        int size;
        if(S_ISLNK(info.st_mode))
            if(!arg.bytes)
                size = info.st_size/block_size; 
            else
                size=info.st_size;
                
        
        else{
            if(!arg.bytes){
                size = info.st_size/block_size;
                if (info.st_size%block_size != 0)
                    size++;
            }
            else
                size=info.st_size;

        }

        printResult(size,pathname);
        return size;
    }
    
}





int  getDirectoryInfo(char * pathname, int max_depth, Args arg)
{

    int sum;
    if (max_depth == 0)
        return callRightFunction(pathname, arg);
    else
    {
        sum=callRightFunction(pathname,arg);
        pid_t pid;
        callRightFunction(pathname, arg);
        DIR * newDir = opendir(pathname); //apontador para os conteudos da pasta
        struct dirent *dp;
       
        char **new_files = malloc(200*sizeof(char*));
        
            
        int idx = 0;
        while((dp = readdir(newDir)) != NULL)
        {
            if ( !strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") )
                continue;
            else 
            {
                new_files[idx] = malloc(200*sizeof(char));
        
                strcpy(new_files[idx], dp->d_name);
                idx++;
                
            }
        }
        closedir(newDir);
    
        char **new_paths = malloc(idx*sizeof(char*));
        for (int i = 0; i < idx; i++)
        {
            new_paths[i] = malloc(200*sizeof(char));
        }
        
        char *newPathname;
        newPathname = pathname;
        strcat(newPathname,"/");
        printf("%s\n", newPathname);
        char *tmp_path = (char*) malloc(200*sizeof(char));
        for (int i = 0; i < idx; i++)
        {

            strcpy(tmp_path, newPathname);
            strcat(tmp_path, new_files[i]);
            strcpy(new_paths[i], tmp_path);
    
        }

        for(int i = 0; i < idx; i++)
        {


            if(isDirectory(new_paths[i]))
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
                    
                    close(fd[READ]);
                    int size = getDirectoryInfo(new_paths[i], max_depth-1,arg);
                    
                    if(!arg.sep_dirs) {
                        write(fd[WRITE],&size,sizeof(size));
                         char temp[30];
                        sprintf(temp, "%d", size);
                        logSendPipe(temp);
                    }
                    
                }
            }
            else
                sum+=callRightFunction(new_paths[i], arg);
         
        }

        for (int i=0; i<idx;i++){
            free(new_files[i]);
            free(new_paths[i]);
        }
        free(new_files);
        free(new_paths);


    }
    
    return sum;

}

int setBlockSize(int size)
{
    block_size = size;
    return 0;
}