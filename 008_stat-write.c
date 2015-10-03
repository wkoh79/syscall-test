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

long global_i;

void *call_stat(void *arg)
{
  struct timespec ts1, ts2;
  struct stat *buf;
  int fd = fileno((FILE *)arg);
  //long i;

  for (global_i = 0; global_i < ITERATION; global_i++)
    MEASURE_ITER("stat", global_i, ts1, ts2, {
                 fstat(fd, buf);}
  );
}

void *modify_stat(void *arg)
{
  FILE *file = (FILE *)arg;
  struct timespec ts1, ts2;
  long i;

  for (i = 0; i < ITERATION; i++)
    MEASURE_ITER("write", i, ts1, ts2, {
                 fprintf(file, "Hello World\n");
                 });
}

void *modify_stat_no_write(void *arg)
{
  FILE *file = (FILE *)arg;
  long i;

  while (global_i < (ITERATION / 2));
    fprintf(file, "Hello World\n");

  fprintf(stdout, "modi stat thread end & stat call #: %ld\n", global_i);
}

int main(int argc, char *argv[])
{
  FILE *file;
  unsigned long status;
  pthread_t thread_call, thread_modi;

  if (argc > 1) {
    fprintf(stderr, "usage: %s\n", argv[0]);
    exit(1);
  }

  file = fopen("stat.txt", "w");
  if (file == NULL) {
    fprintf(stderr, "input file error\n");
    exit(1);
  }

  pthread_create(&thread_call, NULL, call_stat, file);
  //pthread_create(&thread_modi, NULL, modify_stat, file);
  pthread_create(&thread_modi, NULL, modify_stat_no_write, file);

  pthread_join(thread_call, (void **) status);
  pthread_join(thread_modi, (void **) status);

  return 0;
}
