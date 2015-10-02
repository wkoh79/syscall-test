#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "measure.h"

#include <pthread.h>

#define ITERATION 1000

int thread_create_done = 0;

static void *stat_thread_main(void *arg)
{
  struct timespec ts1, ts2;
  struct stat *buf;
  int fd = fileno((FILE *)arg);

  while (!thread_create_done)
    MEASURE_SINGLE("stat", ts1, ts2, 
		{ fstat(fd, buf); });
}

static void *nested_thread_main(void *arg)
{
  pthread_exit((void *)0);
}

static void *create_thread_main(void *arg)
{
  struct timespec ts1, ts2;
  unsigned long status;
  long i = 0;
  int rc;
  pthread_t thread;

  for (i = 0; i < ITERATION; i++) {
    MEASURE_SINGLE("nested_pthread_create", ts1, ts2,
      { pthread_create(&thread, NULL, nested_thread_main, NULL); });

    MEASURE_SINGLE_EXCEPTION("nested_pthread_join", ts1, ts2,
      { rc = pthread_join(thread, (void **)&status); },
      {
        if (rc != 0)
          fprintf(stderr, "error: return code from pthread [%ld] is %d\n", i, rc);
      });
  }
  thread_create_done = 1;

  pthread_exit((void *)0);
}

int main(int argc, char *argv[])
{
  FILE *file;
  unsigned long status;
  pthread_t stat_thread, create_thread;

  if (argc > 1) {
    fprintf(stderr, "usage: %s\n", argv[0]);
    exit(1);
  }

  file = fopen("stat.txt", "w");
  if (file == NULL) {
    fprintf(stderr, "input file error\n");
    exit(1);
  }

  pthread_create(&stat_thread, NULL, stat_thread_main, file);
  pthread_create(&create_thread, NULL, create_thread_main, file);

  pthread_join(stat_thread, (void **) status);
  pthread_join(create_thread, (void **) status);

  return 0;
}
