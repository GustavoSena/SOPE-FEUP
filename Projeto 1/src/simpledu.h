#include <stdio.h> 
#include <signal.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include "args.h"
#include "stat.h"
#include "reg.h"
#define BUFFER_SIZE 512 