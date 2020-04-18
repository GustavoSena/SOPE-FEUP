#include "signal_handlers.h"


extern pid_t child;

void sigint_handler(int signo)
{
    logRecvSig("SIGINT");
    pid_t pid = child;
    char answer;

    if(pid){
		kill(-pid, SIGTSTP);
		logSendSig(-pid, "SIGTSTP");
	}
	
	while(1)
    {
        printf( "Are you sure you want to exit? [Y/N]\n ");
		
		answer = getchar();
		while ( getchar() != '\n' )
            continue;

		if (answer == 'N' || answer == 'n')
		{
            kill(-pid, SIGCONT);
            logSendSig(-pid, "SIGCONT");
            break;  
        }
        else if(answer == 'Y' || answer == 'y')
        {
            kill(-pid, SIGTERM);
            logSendSig(-pid, "SIGTERM");
            logExit(143);
			break;
		}
        else
        {
            printf("Invalid input...\n");
        }
    }
}

void sigcont_handler(int signo)
{
    logRecvSig("SIGCONT");
    printf("Continuing processes...\n");
}

void sigterm_handler(int signo)
{
    logRecvSig("SIGTERM");
    printf("Terminating processes...\n");
    logExit(143);
}


void sigstp_handler(int signo){
	logRecvSig("SIGTSTP");
	pause();
}
