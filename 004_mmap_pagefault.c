#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/mman.h>
#include <string.h>
#include "measure.h"

int main(int argc, char **argv)
{
    char *buf, tmp;
    unsigned long i, buf_size;
    int page_size = 4 << 10;    // 4KiB
    int iterations;

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

    MEASURE_SINGLE_EXCEPTION("mmap", {
                             buf =
                             mmap(NULL, buf_size, PROT_READ | PROT_WRITE,
                                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                             }
                             , if (buf == MAP_FAILED) {
                             printf("failed to mmap: %s\n",
                                    strerror(errno)); return -1;}
    );
    fprintf(stdout, "mmap address: %p\n", buf);

    iterations = buf_size / page_size;
    for (i = 0; i < iterations; i++) {
        MEASURE_ITER("touch", i, {
                     *(buf + (i * page_size)) = 0x55;
                     }
        );
    }

    MEASURE_SINGLE("munmap", {
                   munmap(buf, buf_size);
                   }
    );

    return 0;
}
