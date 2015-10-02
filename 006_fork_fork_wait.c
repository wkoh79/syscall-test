#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "measure.h"

void grandchild_thread(void)
{
  /*fprintf(stderr, "In grandchild process %d\n", getpid());*/
  /*sleep(1);*/
}

void child_thread(int nr_gchildren)
{
  struct timespec ts1, ts2;
  int j;
  int grandchild_status;
  int status = 0;
  pid_t grandchild_pid, wpid;

  /*fprintf(stderr, "create %d grandchildren\n", nr_gchildren);*/
  for (j = 0; j < nr_gchildren; j++) {
    MEASURE_SINGLE_EXCEPTION("fork2", ts1, ts2,
      { grandchild_pid = fork(); },
      if (grandchild_pid == 0) {
        grandchild_thread();
	exit(0);
      });
  }

  /*fprintf(stderr, "wait for grandchildren %d\n", getpid());*/
  do {
    MEASURE_SINGLE_EXCEPTION("wait2", ts1, ts2,
      { wpid = wait(&status); },
      { if (wpid <= 0) break; });
  } while (1);
}

int main(int argc, char **argv)
{
  pid_t child_pid, wpid;
  int status = 0;
  int i;
  int nr_children, nr_gchildren;
  struct timespec ts1, ts2;

  if (argc < 3) {
      printf("usage: %s <nr_children> <nr_grandchildren\n", argv[0]);
      return 0;
  }

  sscanf(argv[1], "%d", &nr_children);
  sscanf(argv[2], "%d", &nr_gchildren);

  setbuf(stdout, NULL);
  /*fprintf(stderr, "create %d children\n", nr_children);*/
  for (i = 0; i < nr_children; i++)
    MEASURE_SINGLE_EXCEPTION("fork", ts1, ts2,
      { child_pid = fork(); },
      if (child_pid == 0) {
        child_thread(nr_gchildren);
        exit(0);
      });

  /*fprintf(stderr, "wait for children %d\n", getpid());*/
  do {
    MEASURE_SINGLE_EXCEPTION("wait", ts1, ts2,
      { wpid = wait(&status); },
      { if (wpid <= 0) break; });
  } while (1);

  return 0;
}
