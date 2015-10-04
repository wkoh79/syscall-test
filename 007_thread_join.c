#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include "measure.h"

int nr_threads, nr_nested_threads;

void *nested_thread_main(void *arg)
{
  /*fprintf(stderr, "In nested threads %d\n", getpid());*/
  /*sleep(1);*/
  pthread_exit((void *)0);
}

static void *thread_main(void *arg)
{
  unsigned long status = 0;
  long i;
  int rc;
  pthread_t *nested_threads;

  nested_threads = malloc(sizeof(pthread_t) * nr_nested_threads);
  if (nested_threads == NULL) {
    fprintf(stderr, "insufficient memory\n");
    pthread_exit((void *)-ENOMEM);
  }

  for (i = 0; i < nr_nested_threads; i++)
    MEASURE_SINGLE("nested_pthread_create",
      { pthread_create(nested_threads + i, NULL, &nested_thread_main, (void *)i); });

  for (i = 0; i < nr_nested_threads; i++)
    MEASURE_SINGLE_EXCEPTION("nested_pthread_join",
      { rc = pthread_join(nested_threads[i], (void **)&status); },
      {
        if (rc != 0)
          fprintf(stderr, "error: return code from pthread [%ld] is %d\n", i, rc);
      });

  free(nested_threads);

  pthread_exit((void *)0);
}

int main(int argc, char **argv)
{
    unsigned long status;
    long i;
    int rc;
    pthread_t *threads;

    if (argc < 3) {
        printf("usage: %s <nr_threads> <nr_nested_threads>\n", argv[0]);
        return 0;
    }

    sscanf(argv[1], "%d", &nr_threads);
    sscanf(argv[2], "%d", &nr_nested_threads);

    threads = malloc(sizeof(pthread_t) * nr_threads);
    if (threads == NULL) {
      fprintf(stderr, "insufficient memory\n");
      return -ENOMEM;
    }

    for (i = 0; i < nr_threads; i++) {
      MEASURE_SINGLE("pthread_create",
        { pthread_create(threads + i, NULL, &thread_main, (void *)i); });
    }

    for (i = 0; i < nr_threads; i++)
      MEASURE_SINGLE_EXCEPTION("pthread_join",
        { rc = pthread_join(threads[i], (void **)&status); },
        {
          if (rc != 0)
            fprintf(stderr, "error: return code from pthread [%ld] is %d\n", i, rc);
        });

    free(threads);

    return 0;
}
