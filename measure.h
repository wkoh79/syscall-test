#ifndef __MEASURE_H__
#define __MEASURE_H__

#include <time.h>

#define MEASURE_ITER(name, iter, ts1, ts2, target_block)\
{\
  clock_gettime(CLOCK_REALTIME, &ts1);\
  { target_block; }\
  clock_gettime(CLOCK_REALTIME, &ts2);\
  \
  fprintf(stdout, "%s elapsed_time: %9.9f ", name, sub_ts(&ts2, &ts1));\
  fprintf(stdout, "[%4ld] ", iter);\
  fprintf(stdout, "start: %ld.%9ld ", ts1.tv_sec, ts1.tv_nsec);\
  fprintf(stdout, "end: %ld.%9ld\n", ts2.tv_sec, ts2.tv_nsec);\
}

#define MEASURE_SINGLE(name, ts1, ts2, target_block)\
{\
  clock_gettime(CLOCK_REALTIME, &ts1);\
  { target_block; }\
  clock_gettime(CLOCK_REALTIME, &ts2);\
  \
  fprintf(stdout, "%s elapsed_time: %9.9f ", name, sub_ts(&ts2, &ts1));\
  fprintf(stdout, "start: %ld.%9ld ", ts1.tv_sec, ts1.tv_nsec);\
  fprintf(stdout, "end: %ld.%9ld\n", ts2.tv_sec, ts2.tv_nsec);\
}

#define MEASURE_SINGLE_EXCEPTION(name, ts1, ts2, target_block, exception)\
{\
  clock_gettime(CLOCK_REALTIME, &ts1);\
  { target_block; }\
  clock_gettime(CLOCK_REALTIME, &ts2);\
  { exception; }\
  \
  fprintf(stdout, "%s elapsed_time: %9.9f ", name, sub_ts(&ts2, &ts1));\
  fprintf(stdout, "start: %ld.%9ld ", ts1.tv_sec, ts1.tv_nsec);\
  fprintf(stdout, "end: %ld.%9ld\n", ts2.tv_sec, ts2.tv_nsec);\
}

static inline double sub_ts(struct timespec *t1, struct timespec *t2)
{
    double ret;

    if (t1->tv_sec == t2->tv_sec) {
        ret = (t1->tv_nsec - t2->tv_nsec) / (double) 1000000000.0;
    } else {
        double t1_double, t2_double;
        t1_double =
            (double) t1->tv_sec +
            (double) t1->tv_nsec / (double) 1000000000.0;
        t2_double =
            (double) t2->tv_sec +
            (double) t2->tv_nsec / (double) 1000000000.0;

        ret = t1_double - t2_double;
    }
    return ret;
}

#endif
