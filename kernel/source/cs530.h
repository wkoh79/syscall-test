#ifndef __CS530_H__
#define __CS530_H__

#define MEASURE_SINGLE(name, target_block) \
{\
	cycle_t start, end;\
	int processor_id = smp_processor_id();\
	start = get_cycles();\
	target_block;\
	end = get_cycles();\
	if (processor_id == smp_processor_id())\
		printk(KERN_DEBUG\
			"  %s elapsed_cycles: %lld\n",\
			name,\
			end - start);\
}
#define MEASURE_SINGLE_COND(name, cond, target_block) \
if (cond) {\
	cycle_t start, end;\
	int processor_id = smp_processor_id();\
	start = get_cycles();\
	target_block;\
	end = get_cycles();\
	if (processor_id == smp_processor_id())\
		printk(KERN_DEBUG\
			"  %s elapsed_cycles: %lld\n",\
			name,\
			end - start);\
}

#endif /* ifdef __CS530_H__ */
