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
    if(block_size == 1024)//se quisermos apenas apresentar o nº real de bytes
    {
        size = info.st_size;
        
    }
    else
    {
        int n_bytes = info.st_size;
        size = n_bytes/block_size;
        if (n_bytes%block_size != 0)
            size++;
    }
    printResult(size, pathname);
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
    if (block_size == 1024)
    {
        size = info.st_size;
    }
    else
    {
        int n_bytes = info.st_size;
        size = n_bytes/block_size;
        
    }
    printResult(size, pathname);
    return 0;
    
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


bool isFile(char * pathname)
{
    struct stat info;
    if(lstat(pathname, &info)<0)
    {
        printf("Error\n");
        exit(0);
    }
    if(S_ISREG(info.st_mode))
        return true;
    else
    {
        return false;
    }

}


int  getDirectoryInfo(char * pathname,int max_depth, Args arg)
{
    if (max_depth == 0)
        getInfo(pathname);
    else
    {
        //int arr[50];
        pid_t pid;
        //int i = 0;
        //int status;
        DIR * newDir = opendir(pathname); //apontador para os conteudos da pasta
        if(readdir(newDir) ==  NULL)
        {
            printf("Error\n");
            exit(1);
        }
        struct dirent *dp;
        char *filename;
        printf("Teste 1\n");
        //Erro - só lê um nome, pode ter que ler vários, adicionar ao codigo de teste do diretorio um for loop antes com um array dos nomes dos ficheiros ou diretorios
        while((dp = readdir(newDir) != NULL))
        {
            if ( !strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") )
                continue;
            else 
            {
                strcpy(filename, dp->d_name);
                //filename = dp->d_name;
            }
        }
        closedir(newDir);
        printf("Teste 2\n");
        char *newPathname = pathname;
        char barra[1] = {'/'};
        strcat(newPathname,barra);
        strcat(newPathname, filename);
        printf("Teste 3\n");
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



            int fd[2];

            if (pipe(fd) < 0) {
                perror("Pipe error!");
                exit(1);
            }


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

                close(fd[WRITE]);
                

                printf("Teste 4 \n");
                //waitpid(-1, &status, WNOHANG); //tira também os processos zombie
                pid_t wtp;
                int status = 0;
                while((wtp = wait(&status))); //espera por todos os processos filhos
                //termina aqui o processo pai
                printf("Teste 5 \n");
                exit(0);

            }
            else //processo filho
            {
                close(fd[READ]);

                //getDirectoryInfo(newPathname,max_depth-1);
                char **commands = get_cmd_args(arg);
                strcpy(commands[0], newPathname);
                printf("Teste 5\n");
                execvp("simpledu", commands);
                printf("Teste 6\n");
                
            }
        }
        else
        {
            getInfo(newPathname); //falta testar se é link simbólico
            printf("Teste 7\n");
        }
        


    }
    
    printf("Teste 8\n");
    return 0;

}

int setBlockSize(int size)
{
    block_size = size;
    return 0;
}