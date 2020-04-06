#pragma once

#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <dirent.h>
#include "args.h"
#include "result.h"

int block_size; 



//Decidir como fazer para apresentar a informação

int getInfo(char * pathname); // se for só para ler o nº de bytes, block_size = 1

bool isSymbolicLink(char * pathname);

int getSymbolicLinkInfo(char * pathname);

bool isDirectory(char * pathname);

bool isFile(char * pathname);

int getDirectoryInfo(char * pathname, int max_depth, Args arg); 

int setBlockSize(int size);