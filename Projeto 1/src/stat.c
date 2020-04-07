#include "stat.h"


static int block_size; 
int getInfo(char * pathname) //Usar isto para symbolic links se -L for passada como argumento e para ficheiros
{
    struct stat info;
    if(lstat(pathname, &info)<0)
    {
        printf("Error\n");
        exit(1);
    }
    
    
    int n_bytes = info.st_size;
    int size = n_bytes/block_size;
    if (n_bytes%block_size != 0)
        size++;
    
    printResult(size,pathname);
    return size;
    
}


bool isSymbolicLink(char * pathname)
{
    struct stat info;
    if(lstat(pathname, &info)<0)
    {
        perror("Error\n");
        exit(1);
    }
    return S_ISLNK(info.st_mode);
}

int getSymbolicLinkInfo(char * pathname) 
{
    struct stat info;
    if(lstat(pathname, &info)<0)
    {
        perror("Error\n");
        exit(1);
    }
    
    
    int n_bytes = info.st_size;
    int size = n_bytes/block_size;
    printResult(size,pathname);
    return size;
    
}

bool isDirectory(char * pathname)
{
    struct stat info;
    if(lstat(pathname, &info)<0)
    {
        perror("Error\n");
        exit(1);
    }
    return S_ISDIR(info.st_mode);


}
bool isFile(char * pathname)
{
    struct stat info;
    if(lstat(pathname, &info)<0)
    {
        perror("Error\n");
        exit(1);
    }
    return S_ISREG(info.st_mode);
     
}


int callRightFunction(char * pathname, Args arg)
{



    /*if(isFile(pathname))
    {
        return getInfo(pathname);
    }
    else if(isSymbolicLink(pathname))
    {
        printf ("esta aqui\n");
        if (arg.dereference) //se o argumento tem -L
            return getInfo(pathname);
        else
        {
            return getSymbolicLinkInfo(pathname);
           
        }
        
    }*/

    if (arg.dereference){
        struct stat info;
        if(stat(pathname, &info)<0)
        {
            printf("Error\n");
            exit(1);
        }
        int n_bytes;
        int size;
        if(S_ISLNK(info.st_mode)){
            printf("Entrou\n");
            n_bytes = info.st_size;
            size = n_bytes/block_size; 
        }
        else{
            n_bytes = info.st_size;
            size = n_bytes/block_size;
            if (n_bytes%block_size != 0)
                size++;
        }
        printResult(size,pathname);
        return size;
    }
    else{
        struct stat info;
        if(lstat(pathname, &info)<0)
        {
            printf("Error\n");
            exit(1);
        }
        
        
        int n_bytes = info.st_size;
        int size = n_bytes/block_size;
        
        
        printResult(size,pathname);
        return size;
    }
    
}





int  getDirectoryInfo(char * pathname,
int max_depth, Args arg)
{

    int sum;
    if (max_depth == 0)
        return callRightFunction(pathname, arg);
    else
    {
        sum=getInfo(pathname);
        pid_t pid;
        getInfo(pathname);
        DIR * newDir = opendir(pathname); //apontador para os conteudos da pasta
        struct dirent *dp;
        //printf("Teste 1\n");
       
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
                printf("%s\n", new_files[idx]);
                idx++;
            }
        }
        closedir(newDir);
        //printf("Teste 2\n");

        char **new_paths = malloc(idx*sizeof(char*));
        for (int i = 0; i < idx; i++)
        {
            new_paths[i] = malloc(200*sizeof(char));
        }
        
        char *newPathname;
        char barra[1] = {'/'};
        newPathname = pathname;
        strcat(newPathname,barra);
        char *tmp_path = (char*) malloc(200*sizeof(char));
        for (int i = 0; i < idx; i++)
        {
            strcpy(tmp_path, newPathname);
            strcat(tmp_path, new_files[i]);
            strcpy(new_paths[i], tmp_path);
            //printf(new_paths[i]);
            //printf("\n");
            //strcpy(tmp_path, newPathname);
        }

        //printf("Teste 3\n");

        for(int i = 0; i < idx; i++)
        {


            if(isDirectory(new_paths[i]))
            {
                int fd[2];

                if (pipe(fd) < 0) {
                    perror("Pipe error!");
                    exit(1);
                }
                pid = fork();
                if (pid < 0)
                {
                    perror("Error\n");
                    exit(1);
                }
                else if (pid > 0) //processo pai
                {

                    close(fd[WRITE]);
                   // printf("Teste 4 \n");
                    //pid_t wtp;
                    int status = 0;
                    waitpid(-1, &status, 0);
                        
                    //termina aqui o processo pai
                   
                    int s;
                    int temp=0;
                    if(!arg.sep_dirs){
                        read(fd[READ],&s,sizeof(int));
                        
                        temp+=s;
                        sum+=s;
                    }
                    //printResult(temp,newPathname);
                    

                    //exit(0);

                }
                else //processo filho
                {
                    
                    //int n;
                    //read(fd[READ], &n, sizeof(int));
                    //getDirectoryInfo(newPathname,max_depth-1);
                    // char **commands = get_cmd_args(arg);
                    // strcpy(commands[0], new_paths[i]);
                    // printf("Teste 6\n");
                    // execvp("simpledu", commands);
                    // printf("Teste 7\n");
                    /*Args new_args = arg;
                    new_args.max_depth--;*/
                    //strcpy(new_args.path, new_paths[i]);

                    close(fd[READ]);
                    int size =getDirectoryInfo(new_paths[i], max_depth-1,arg);
                    
                    if(!arg.sep_dirs)
                        write(fd[WRITE],&size,sizeof(size));
                    
                    
                }
            }
            else
            {
                int s=callRightFunction(new_paths[i], arg);
                //if(!arg.sep_dirs)
                    sum+=s;
                
                
            }
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