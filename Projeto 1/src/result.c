#include "result.h"

void printResult(int result , char * dir)
{
   char res[12];
   sprintf(res, "%d", result);
   write(STDOUT_FILENO, &res, strlen(res));
   write(STDOUT_FILENO, "       ", 8);
   write(STDOUT_FILENO, dir, strlen(dir));
   write(STDOUT_FILENO, "\n", 2);
}