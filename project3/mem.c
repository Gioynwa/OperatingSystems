#include <sys/syscall.h>
#include <unistd.h>
#include "mem.h"

long slob_get_total_alloc_mem_wrapper(void) {
	
	return( syscall(__NR_slob_get_total_alloc_mem) );
}
