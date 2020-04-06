#include "reg.h"

clock_t beginTime;
FILE *logs;

void initLogs(){
    beginTime = clock();
    setenv("LOG_FILENAME","logs.txt",0);

    if((logs = fopen(getenv("LOG_FILENAME"),"w"))==NULL){
        perror("Unable to open file");
        exit(1);
    }
}


void createReg(Reg * reg,char *action){
    
    reg->time=((double)(clock()-beginTime))/(CLOCKS_PER_SEC/(double)1000.0);
    reg->pid=getpid();
    reg->action=action;
    strncpy(reg->info,"Command: ", sizeof("Command: "));
}

void writeReg(Reg * reg){
    fprintf(logs,"%.2f - %.8d - %s - %s\n", reg->instant, reg->pid, reg->action, reg->info)
}

void logCreate(int argc, char *args[]) {
    Reg reg; 
    createReg(&reg, "CREATE");
    for(int i = 0; i < argc; i++){
		strcat(reg.info, args[i]);
        if (i != argc-1)
            strcat(reg.info," ");
	}
    writeReg(&reg);
}

void logExit(int status) {
    Reg reg; 
    createReg(&reg, "EXIT");
    sprintf(reg.info, "%d", status);
    writeReg(&reg);
    exit(status);
}

void logRecvSig(int sig) {
    Reg reg; 
    createReg(&reg, "RECV_SIGNAL");
    sprintf(reg.info, "%d", sig);
    writeReg(&reg);
}

void logSendSig(pid_t pid, int sig) {
    Reg reg; 
    createReg(&reg, "SEND_SIGNAL");
    sprintf(reg.info, "Sent %d to process %d", sig, pid);
    writeReg(&reg);
}
