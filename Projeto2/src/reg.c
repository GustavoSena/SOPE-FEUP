#include "reg.h"






void writeReg(Request rq, char* order){
    fprintf(stdout,"%.10ld ; %.4d ; %.6d ; %.6d ; %.6d ; %.4d ;%s\n", time(NULL), rq.i, rq.pid ,rq.tid , rq.dur, rq.pl, order);
    fflush(stdout);
}

void logWant(Request rq) {
    writeReg(rq,"IWANT");
}

void logRecv(Request rq) {
    writeReg(rq,"RECVD");
}

void logEnter(Request rq) {
    writeReg(rq,"ENTER");
}

void logIamIn(Request rq) {
    writeReg(rq,"IAMIN");
}

void logTimeUp(Request rq) {
    writeReg(rq,"TIMEUP");
}

void log2Late(Request rq) {
    writeReg(rq,"2LATE");
}

void logClosed(Request rq) {
    writeReg(rq,"CLOSD");
}

void logFailed(Request rq) {
    writeReg(rq,"FAILD");
}   

void logGaveUp(Request rq) {
    writeReg(rq,"GAVUP");
}




/*
void logExit(int status) {
    Reg reg; 
    createReg(&reg, "EXIT");
    sprintf(reg.info, "%d", status);
    writeReg(&reg);
    exit(status);
}

void logRecv(char * sig) {
    Reg reg; 
    createReg(&reg, "RECV_SIGNAL");
    sprintf(reg.info, "%s", sig);
    writeReg(&reg);
}

void logSendSig(pid_t pid,char *sig) {
    Reg reg; 
    createReg(&reg, "SEND_SIGNAL");
    sprintf(reg.info, "Sent %s to process %d", sig, pid);
    writeReg(&reg);
}


void logRecvPipe(char* info) {
    Reg reg; 
    createReg(&reg, "RECV_PIPE");
    strcat(reg.info,info);
    writeReg(&reg);
    
}

void logSendPipe(char* message) {
    Reg reg; 
    createReg(&reg, "SEND_PIPE");
    strcat(reg.info,message);
    writeReg(&reg);
}

void logEntry(int size, char* path) {
    Reg reg; 
    createReg(&reg, "ENTRY");
    sprintf(reg.info, "%d %s", size, path);

    writeReg(&reg);
}

void logEntry(int size, char* path) {
    Reg reg; 
    createReg(&reg, "ENTRY");
    sprintf(reg->info, "%d %s", size, path);

    writeReg(&reg);
}

	strcat(reg.info, new_path);
    strcat(reg.info, "-l ");
	strcat(reg.info, argline(arg));
	writeReg(&reg);
}

*/