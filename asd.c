#include <stdio.h>
#include <string.h>
#include <stdlib.h> // For: malloc(), realloc(), free(), exit(), execvp(), EXIT_SUCCESS, EXIT_FAILURE
#include <time.h>
#include <sys/time.h>
#include <sys/timeb.h>

int main(int argc, char *argv[])

{
    for (int i = 1; i < 10; i++)
    {
        printf(" %d", i);
    }
}