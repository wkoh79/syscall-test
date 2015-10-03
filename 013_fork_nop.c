#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include "measure.h"

int fork_done = 0;

void *yield_thread(void *arg)
{
  struct timespec ts1, ts2;
  struct stat *buf;

  while (!fork_done)
    pthread_yield();
    /*sleep(1);*/
  pthread_exit((void *)0);
}

void child_main(void)
{
}

int main(int argc, char **argv)
{
  pid_t child_pid, wpid;
  int status = 0;
  int i;
  int nr_children;
  struct timespec ts1, ts2;

  FILE *file;

  pthread_t thread_call;
  unsigned long thread_status;

  if (argc < 2) {
    printf("usage: %s <nr_children>\n", argv[0]);
    return 0;
  }

  sscanf(argv[1], "%d", &nr_children);
  setbuf(stdout, NULL);

  file = fopen("stat.txt", "w");
  if (file == NULL) {
    fprintf(stderr, "input file error\n");
    exit(1);
  }

  pthread_create(&thread_call, NULL, yield_thread, NULL);

  for (i = 0; i < nr_children; i++)
    MEASURE_SINGLE_EXCEPTION("fork", ts1, ts2,
      { child_pid = fork(); },
      if (child_pid == 0) {
        child_main();
        exit(0);
      } else if (child_pid < 0) {
        fprintf(stderr, "failed to fork a child %d\n", i);
	break;
      }
    );
  fork_done = 1;

  /*fprintf(stderr, "wait for children\n");*/
  do {
    MEASURE_SINGLE_EXCEPTION("wait", ts1, ts2,
        { wpid = wait(&status); },
        { if (wpid <= 0) break; });
    /*printf("Exit status of %d was %d\n", (int) wpid, status);*/
  } while (1);

  pthread_join(thread_call, (void **) thread_status);
  fclose(file);

  return 0;
}
