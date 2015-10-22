#ifndef __CS530_H__
#define __CS530_H__

#if 0
#define tsc_start(start) \
	{ start = get_cycles(); }
#define tsc_end(end) \
	{ end = get_cycles(); }
#else
#define tsc_start(start) \
	__asm__ __volatile__ \
	("cpuid\n\trdtsc\n\tshlq $32, %%rdx\n\torq %%rdx, %%rax\n\tmovq %%rax, %0":\
	 "=r"(start): : "rax", "rbx", "rdx", "rcx")
#define tsc_end(end) \
	__asm__ __volatile__ \
	("rdtscp\n\tshlq $32, %%rdx\n\torq %%rdx, %%rax\n\tmovq %%rax, %0\n\tcpuid":\
	 "=r"(end): : "rax", "rbx", "rdx", "rcx")
#endif
#define MEASURE_SINGLE(name, target_block) \
{\
	if (strncmp(current->comm, "t12_", 4) == 0) {\
		cycle_t start, end;\
		static unsigned long sum = 0;\
		static unsigned long sqrd_sum = 0;\
		static unsigned long count = 0;\
		tsc_start(start);\
		target_block;\
		tsc_end(end);\
		sum = sum + (end - start);\
		sqrd_sum = sqrd_sum + ((end - start) * (end - start));\
		count++;\
		if ((count % 100) == 0)\
			printk(KERN_DEBUG\
				"%s %s elapsed_cycles: %lld sum: %ld"\
				" sqrd_sum: %ld count: %ld\n",\
				current->comm,\
				name,\
				end - start, sum, sqrd_sum, count);\
	} else {\
		target_block;\
	}\
}
#define MEASURE_SINGLE_COND(name, cond, target_block) \
if (cond) {\
	if (strncmp(current->comm, "t12_", 4) == 0) {\
		cycle_t start, end;\
		static unsigned long sum = 0;\
		static unsigned long sqrd_sum = 0;\
		static unsigned long count = 0;\
		tsc_start(start);\
		target_block;\
		tsc_end(end);\
		sum = sum + (end - start);\
		sqrd_sum = sqrd_sum + ((end - start) * (end - start));\
		count++;\
		if ((count % 100) == 0)\
			printk(KERN_DEBUG\
				"%s %s elapsed_cycles: %lld sum: %ld"\
				" sqrd_sum: %ld count: %ld\n",\
				current->comm,\
				name,\
				end - start, sum, sqrd_sum, count);\
	} else {\
		target_block;\
	}\
}

#endif /* ifdef __CS530_H__ */
