#include "mem.h"
#include "free.h"
#include <stdio.h>
#define NO 20

int main(int argc, char *argv[]) {

	long alloc, free;
	int i;	
	
	for(i = 0; i < NO; i++) {	
		alloc = slob_get_total_alloc_mem_wrapper();
		free = slob_get_total_free_mem_wrapper();
		printf("alloc: %ld\n", alloc);
		printf("free: %ld\n", free);
	}
	
	return(0);
}

