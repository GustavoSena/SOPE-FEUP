#include "result.h"

void printResult(int * results, char ** dirs, int size)
{
    for(int i = 0; i < size; i++)
    {
        write(STDOUT_FILENO, &results[i], sizeof(results[i]));
        write(STDOUT_FILENO, "      ", 7);
        write(STDOUT_FILENO, dirs[i], sizeof(dirs[i]));
        write(STDOUT_FILENO, "\n", 2);
    }
}