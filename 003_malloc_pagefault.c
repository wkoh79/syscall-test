#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include "measure.h"

int main(int argc, char **argv)
{
    char *buf, tmp;
    unsigned long i, buf_size;
    int page_size = 4 << 10;    // 4KiB
    int iterations;
    struct timespec ts1, ts2;

    if (argc < 2) {
        printf("usage: %s <mem_size_in_MB> <page_size_in_KB>\n", argv[0]);
        return 0;
    }

    sscanf(argv[1], "%ld", &buf_size);
    printf("mem_size: %ld MiB\n", buf_size);
    buf_size <<= 20;

    if (argc == 3) {
        sscanf(argv[2], "%d", &page_size);
        page_size <<= 10;
        if (page_size != (4 << 10) && page_size != (2 << 20)) {
            printf("invalied page size. 4KiB and 2MiB supported\n");
            return -EINVAL;
        }
    }

    MEASURE_SINGLE_EXCEPTION("malloc", ts1, ts2, {
                             buf = malloc(buf_size);}
                             , if (!buf) {
                             printf("not sufficient memory: %ld\n",
                                    buf_size >> 20); return -ENOMEM;}
    );

    iterations = buf_size / page_size;
    for (i = 0; i < iterations; i++)
        MEASURE_ITER("touch", i, ts1, ts2, {
                     *(buf + (i * page_size)) = 0x55;}
    );

    MEASURE_SINGLE("free", ts1, ts2, {
                   free(buf);}
    );
    return 0;
}
