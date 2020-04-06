#include "stat.h"

int getFileInfo(char * pathname, int block_size) 
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

int getSymbolicLinkInfo(char * pathname, int block_size) 
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

int getDirectoryInfo(char * pathname, int block_size, int max_depth)
{
    
}