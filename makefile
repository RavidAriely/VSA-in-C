CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra 

VSA = vsa.c vsa_test.c vsa.h

DEBUG = yes



vsa: $(OBJECTS)
	@$(CC) $(CFLAGS) $(VSA) -o vsa.out
	
make.o_vsa: $(OBJECTS)
	@$(CC) $(CFLAGS) $(VSA) -c 
	
clean_vsa: 
	@rm *.out *.o
	
all_vsa: debug make.o make
	
debug_vsa: $(OBJECTS)
	@$(CC) $(CFLAGS) $(VSA) -DDEBUG -g -o vsa_debug.out 
	
release_vsa: $(OBJECTS)
	@$(CC) $(CFLAGS) $(VSA) -O3 -o vsa_release.out 
	
	
	
