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
    strcpy(reg->action, action);
    strcpy(reg->info,"Command: ");
}

void writeReg(Reg * reg){
    fprintf(logs,"%.2f - %.8d - %s - %s\n", reg->time, reg->pid, reg->action, reg->info);
    setbuf(logs, NULL); //limpar o buffer
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

void logSendSig(pid_t pid,int sig) {
    Reg reg; 
    createReg(&reg, "SEND_SIGNAL");
    sprintf(reg.info, "Sent %d to process %d", sig, pid);
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


void logCreateFork(Args arg, int max_depth,char * new_path){
	arg.max_depth = max_depth;
    
	Reg reg;
	createReg(&reg, "CREATE");

	strcat(reg.info, new_path);
    strcat(reg.info, "-l ");
	strcat(reg.info, argline(arg));
	writeReg(&reg);
}

