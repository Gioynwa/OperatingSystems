all: slob_get_total_mem_lib

slob_get_total_mem_lib: mem.c free.c
	cc -Wall -c mem.c
	cc -Wall -c free.c
	ar -cvq libmem.a mem.o free.o
	ar -t libmem.a 
	cc -o slob_get_total_mem_lib slob_get_total_mem_lib.c libmem.a
	
