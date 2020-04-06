#include "stat.h"

int getInfo(char * pathname) //Usar isto para symbolic links se -L for passada como argumento e para ficheiros
{
    struct stat info;
    if(stat(pathname, &info)<0)
    {
        printf("Error\n");
        exit(1);
    }
    int size;
    if(block_size == 1)//se quisermos apenas apresentar o nº real de bytes
    {
        size = info.st_size;
        
    }
    else
    {
        int n_bytes = info.st_size;
        if (n_bytes%block_size==0)
            size = n_bytes/block_size;
        else
        {
            size = n_bytes/block_size + 1;
        }
        
    }
    return size;
    
}


bool isSymbolicLink(char * pathname)
{
    struct stat info;
    if(lstat(pathname, &info)<0)
    {
        printf("Error\n");
        exit(1);
    }
    if(S_ISLNK(info.st_mode))
        return true;
    else
    {
        return false;
    }
    

}

int getSymbolicLinkInfo(char * pathname) 
{
    struct stat info;
    if(lstat(pathname, &info)<0)
    {
        printf("Error\n");
        exit(0);
    }
    int size;
    if (block_size == 1)
    {
        size = info.st_size;
    }
    else
    {
        int n_bytes = info.st_size;
        if (n_bytes%block_size==0)
            size = n_bytes/block_size;
        else
        {
            size = n_bytes/block_size + 1;
        }
        
    }
    return size;
    
}

bool isDirectory(char * pathname)
{
    struct stat info;
    if(lstat(pathname, &info)<0)
    {
        printf("Error\n");
        exit(0);
    }
    if(S_ISDIR(info.st_mode))
        return true;
    else
    {
        return false;
    }

}


int  getDirectoryInfo(char * pathname,int max_depth, Args arg)
{
    if (max_depth == 0)
        return getInfo(pathname);
    else
    {
        int arr[50];
        pid_t pid;
        int i = 0;
        int status;
        DIR * newDir = opendir(pathname); //apontador para os conteudos da pasta
        if(readdir(newDir) ==  NULL)
        {
            printf("Error\n");
            exit(1);
        }
        struct dirent *dp;
        char *filename;
        while((dp = readdir(newDir) != NULL))
        {
            if ( !strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") )
                continue;
            else 
            {
                filename = dp->d_name;
            }
        }
        closedir(newDir);
        char *newPathname = pathname;
        char barra[1] = {'/'};
        strcat(newPathname,barra);
        strcat(newPathname, filename);

        //usar o readdir //Ignorar o . e o .. dá o nome da pasta com d name da stat
        /**
         * d = opendir(".");
        if (d == NULL) {
            fprintf(stderr, "Couldn't open \".\"\n");
            exit(1);
            }

        for (de = readdir(d); de != NULL; de = readdir(d)) {
  
        }
        closedir(d);
        */
        if(isDirectory(newPathname))
        {
            pid = fork();
            //criar exec-instanciação do simpledu  com a nova pasta no processo filho e passar o novo path com todos os argumentos e subtrair o max_depth - 1
            //cuidado para ir buscar o nome do path +/<nomedoficheiro>
            if (pid < 0)
            {
                printf("Error\n");
                exit(1);
            }
            else if (pid > 0) //processo pai
            {
                //waitpid(-1, &status, WNOHANG); //tira também os processos zombie
                pid_t wtp;
                int status = 0;
                while((wtp = wait(&status)));
                //termina aqui o processo pai
                exit(0);

            }
            else //processo filho
            {
                //getDirectoryInfo(newPathname,max_depth-1);
                char commands[7];
                execvp("simpledu", commands);
                
            }
        }
        else
        {
            return getInfo(newPathname);
        }
        


    }
    

}