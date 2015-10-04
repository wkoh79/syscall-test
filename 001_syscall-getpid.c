#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "measure.h"

#define ITERATION 1000

int main(int argc, char *argv[])
{
    long i;
    unsigned long int pid;

    if (argc > 1) {
        fprintf(stderr, "usage: %s\n", argv[0]);
        exit(1);
    }

    for (i = 0; i < ITERATION; i++)
        MEASURE_ITER("getpid", i, {getpid();});

    return 0;
}
