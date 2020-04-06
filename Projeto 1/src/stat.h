#include <stdio.h> 
#include <signal.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <stdbool.h>


//Decidir como fazer para apresentar a informação

int getFileInfo(char * pathname, int block_size); // se for só para ler o nº de bytes, block_size = 1

bool isSymbolicLink(char * pathname);

int getSymbolicLinkInfo(char * pathname, int block_size);

bool isDirectory(char * pathname);

int getDirectoryInfo(char * pathname, int block_size, int max_depth); //não percebo como vamos buscar a informação dos diretórios