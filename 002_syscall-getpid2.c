#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "measure.h"

#define ITERATION 1000

int main(int argc, char *argv[])
{
    struct timespec ts1, ts2;
    long i;
    unsigned long int pid;

    if (argc > 1) {
        fprintf(stderr, "usage: %s\n", argv[0]);
        exit(1);
    }

    for (i = 0; i < ITERATION; i++)
        MEASURE_ITER("getpid_w/o_TLS", i, ts1, ts2, {
                     asm volatile ("syscall":"=a" (pid):"0"(39):"memory",
                                   "cc", "r11", "cx");});

    return 0;
}
