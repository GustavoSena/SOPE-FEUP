#include "signal_handlers.h"

extern pid_t main_prg;

void sigint_handler(int signo)
{
    logRecvSig(SIGINT);
    pid_t pid = main_prg;
    char answer;
    while(1)
    {
        printf( "Are you sure you want to exit? [Y/N]\n ");
		answer = getchar();
        while ( getchar() != '\n' ){}
		if (answer == 'N'|| answer == 'n')
		{
            logSendSig(pid, SIGCONT);
            kill(-pid, SIGCONT);
            break;  
        }
        else if(answer == 'Y' || answer == 'y')
        {
            logSendSig(pid, SIGTERM);
            kill(-pid, SIGTERM);
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
    logRecvSig(SIGCONT);
    printf("Continuing processes...\n");
}

void sigterm_handler(int signo)
{
    logRecvSig(SIGTERM);
    printf("Terminating processes...\n");
    logExit(143);
}
