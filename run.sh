#!/bin/bash
make clean; make

echo "                   min          max          avg      count    outlier      stdev"
echo -n "case 001  "
./001_syscall-getpid |awk -f cal_sd.awk
echo -n "case 002  "
./002_syscall-getpid2  |awk -f cal_sd.awk
echo -n "case 003  "
./003_malloc_pagefault 8 4 |egrep "^touch " |awk -f cal_sd.awk
echo -n "case 004  "
./004_mmap_pagefault 8 4 |egrep "^touch "|awk -f cal_sd.awk
echo -n "case 005  "
./005_fork_wait 1000 |egrep "^fork " |awk -f cal_sd.awk
echo -n "case 006  "
./006_fork_fork_wait 100 100 |egrep "^fork2 " |awk -f cal_sd.awk
echo -n "case 007  "
./007_thread_join 100 100 |egrep "^nested_pthread_create " |awk -f cal_sd.awk
echo -n "case 008  "
./008_stat-write |awk -f cal_sd.awk
echo -n "case 009  "
./009_pthread_stat |egrep "^stat " |awk -f cal_sd.awk
echo -n "case 010  "
./010_fork_stat 1000 |egrep "^stat " |awk -f cal_sd.awk
echo -n "case 011  "
./011_fork_write 100 |egrep "^write " |awk -f cal_sd.awk
echo -n "case 012-1"
./012_fork_dup 10000 |egrep "^fork " |awk -f cal_sd.awk
echo -n "case 012-2"
./012_fork_dup 10000 |egrep "^dup " |awk -f cal_sd.awk
echo -n "case 013  "
./013_fork_nop 10000 |egrep "^fork " |awk -f cal_sd.awk
