#pragma once

#include <stdio.h> 
#include <signal.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include "reg.h"
#include "reg.h"

void sigint_handler(int signo);

void sigcont_handler(int signo);

void sigterm_handler(int signo);
