#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "measure.h"

int main(int argc, char **argv)
{
  pid_t child_pid, wpid;
  int status = 0;
  int i;
  int nr_children;

  if (argc < 2) {
    printf("usage: %s <nr_children>\n", argv[0]);
    return 0;
  }

  sscanf(argv[1], "%d", &nr_children);
  setbuf(stdout, NULL);

  /*fprintf(stderr, "create %d children\n", nr_children);*/

  for (i = 0; i < nr_children; i++)
    MEASURE_SINGLE_EXCEPTION("fork",
      { child_pid = fork(); },
      if (child_pid == 0) {
        /*printf("In child process (pid: %d)\n", getpid());*/
        exit(0);
      }
    );

  /*fprintf(stderr, "wait for children\n");*/
  do {
    MEASURE_SINGLE_EXCEPTION("wait",
        { wpid = wait(&status); },
        { if (wpid <= 0) break; });
    /*printf("Exit status of %d was %d\n", (int) wpid, status);*/
  } while (1);

  return 0;
}
