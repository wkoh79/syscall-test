#!/bin/bash
./001_syscall-getpid 
./002_syscall-getpid2 
./003_malloc_pagefault 1024 2048
./004_mmap_pagefault 1024 2048
./005_fork_wait 100
./006_fork_fork_wait 100 100
./007_thread_join 100 100
