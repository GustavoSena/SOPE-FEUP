#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <inttypes.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "utils.h"







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
