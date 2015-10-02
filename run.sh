#!/bin/bash
echo "case 001"
./001_syscall-getpid |awk -f cal_sd.awk
echo "case 002"
./002_syscall-getpid2  |awk -f cal_sd.awk
echo "case 003"
./003_malloc_pagefault 1024 2048 |egrep "^touch " |awk -f cal_sd.awk
echo "case 004"
./004_mmap_pagefault 1024 2048 |egrep "^touch "|awk -f cal_sd.awk
echo "case 005"
./005_fork_wait 100 |egrep "^fork " |awk -f cal_sd.awk
echo "case 006"
./006_fork_fork_wait 100 100 |egrep "^fork2 " |awk -f cal_sd.awk
echo "case 007"
./007_thread_join 100 100 |egrep "^nested_pthread_create " |awk -f cal_sd.awk
echo "case 008"
./008_stat-write |awk -f cal_sd.awk
echo "case 009"
./009_pthread_stat |egrep "^stat " |awk -f cal_sd.awk
