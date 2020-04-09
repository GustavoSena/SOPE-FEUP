#include "signal_handlers.h"



void sigint_handler(int signo)
{
    pid_t pid = getpid();
    char answer[2];
    write(STDOUT_FILENO, "Are you sure you want to exit? [Y/N]\n ", 39);
    read(STDIN_FILENO, answer, 2*sizeof(char));
    if(strcmp(answer, "N") == 0 || strcmp(answer, "n") == 0)
    {
        
    }
    
}

void sigcont_handler(int signo)
{
    write(STDOUT_FILENO, "Continuing processes...\n", 25);
}

void sigterm_handler(int signo)
{
    write(STDOUT_FILENO, "Terminating processes...\n", 26);
    logExit(0); //não sei qual é suposto ser o código de terminação
}