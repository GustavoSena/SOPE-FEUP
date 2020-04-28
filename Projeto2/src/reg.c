#include "reg.h"


void writeReg(Request rq, char* order){
    fprintf(stdout,"%.10ld ; %.4d ; %.6d ; %.6ld ; %.6d ; %.4d ;%s\n", time(NULL), rq.i, rq.pid ,rq.tid , rq.dur, rq.pl, order);
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





