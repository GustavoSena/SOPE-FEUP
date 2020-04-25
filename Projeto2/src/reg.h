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
#include "utils.h"






void initLogs();
void writeReg(Request rq, char* order);
void logWant(Request rq);
void logRecv(Request rq);
void logEnter(Request rq);
void logIamIn(Request rq);
void logTimeUp(Request rq);
void log2Late(Request rq);
void logClosed(Request rq);
void logFailed(Request rq);
void logGaveUp(Request rq);
/*
void createReg(Reg * reg,char action[]);

void logCreate(int argc, char *argv[]);
void logExit(int status);
void logRecvSig(char * sig);
void logSendSig(pid_t pid,char * sig);
void logRecvPipe(char* info);
void logSendPipe(char* message);
void logEntry(int size, char *path);
void logCreateFork(Args arg, int max_depth, char *new_path);

*/
