#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <inttypes.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "args.h"


typedef struct {
    char info[256];
    char action[64];
    double time;
    pid_t pid;
} Reg;

void initLogs();
void createReg(Reg * reg,char action[]);
void writeReg(Reg * reg);
void logCreate(int argc, char *argv[]);
void logExit(int status);
void logRecvSig(int sig);
void logSendSig(pid_t pid,int sig);
void logRecvPipe(char* info);
void logSendPipe(char* message);
void logEntry(int size, char *path);
void logCreateFork(Args arg, int max_depth, char *new_path);