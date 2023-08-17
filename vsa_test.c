#include <stdlib.h>    /* malloc */
#include <stdio.h>
#include "vsa.h"
#include "utilities.h"

int main()
{
	/* test case 1 */
	char *new_ptr= NULL;
	vsa_t *new_vsa = NULL;
	void *alloc_return_1 = NULL;
	void *alloc_return_2 = NULL;
	void *alloc_return_3 = NULL;
	void *alloc_return_4 = NULL;
	void *alloc_return_5 = NULL;
	
	/* test case 2 */
	char *new_ptr_2= NULL;
	vsa_t *new_vsa_2 = NULL;
	void *alloc_return_10 = NULL;
	void *alloc_return_11 = NULL;
	void *alloc_return_12 = NULL;
	void *alloc_return_13 = NULL;
	void *alloc_return_14 = NULL;
	
	
	/*
	 * TEST CASE 1 : unaligned address
	 */
	printf("-----------------\n");
	printf("TEST CASE 1");
	printf("\n-----------------\n\n");
	 
	new_ptr = malloc(240);

	printf("INIT:\n");
	new_vsa = VsaInit(new_ptr+3, 240);
	TESTS(VsaLargestChunk(new_vsa) == 195);
	
	printf("\nALLOC 1:\n");
	alloc_return_1 = VsaAlloc(new_vsa, 25);
	TESTS(alloc_return_1 != NULL);
	TESTS(VsaLargestChunk(new_vsa) == 139);
	
	printf("\nALLOC 2:\n");
	alloc_return_2 = VsaAlloc(new_vsa, 24);
	TESTS(alloc_return_2 != NULL);
	TESTS(VsaLargestChunk(new_vsa) == 91);

	printf("\nALLOC 3:\n");
	alloc_return_3 = VsaAlloc(new_vsa, 25);
	TESTS(alloc_return_3 != NULL);
	TESTS(VsaLargestChunk(new_vsa) == 35);
	
	printf("\nALLOC 4:\n");
	alloc_return_4 = VsaAlloc(new_vsa, 5);
	TESTS(alloc_return_4 != NULL);
	TESTS(VsaLargestChunk(new_vsa) == 0);
	
	printf("\nFREE 3:\n");
	VsaFree(alloc_return_3);
	TESTS(VsaLargestChunk(new_vsa) == 32);
	
	printf("\nFREE 2:\n");
	VsaFree(alloc_return_2);
	TESTS(VsaLargestChunk(new_vsa) == 80);
	
	printf("\nALLOC AGAIN 1:\n");
	alloc_return_2 = VsaAlloc(new_vsa, 24);
	TESTS(alloc_return_2 != NULL);
	TESTS(VsaLargestChunk(new_vsa) == 32);
	
	printf("\nALLOC AGAIN 2:\n");
	alloc_return_3 = VsaAlloc(new_vsa, 25);
	TESTS(alloc_return_3 != NULL);
	TESTS(VsaLargestChunk(new_vsa) == 0);

	printf("\nALLOC AGAIN 5:\n");
	alloc_return_5 = VsaAlloc(new_vsa, 300);
	TESTS(alloc_return_5 == NULL);
	TESTS(VsaLargestChunk(new_vsa) == 0);
	
	free(new_ptr);
	
	

	
	/*
	 * TEST CASE 12 : aligned address
	 */
	printf("\n\n-----------------\n");
	printf("TEST CASE 2");
	printf("\n-----------------\n\n");
	
	new_ptr_2 = malloc(315);
	printf("INIT:\n");
	new_vsa_2 = VsaInit(new_ptr_2, 315);
	TESTS(VsaLargestChunk(new_vsa_2) == 275);
	
	printf("\nALLOC 10:\n");	
	alloc_return_10 = VsaAlloc(new_vsa_2, 15);
	TESTS(alloc_return_10 != NULL);	
	TESTS(VsaLargestChunk(new_vsa_2) == 235);		
	
	printf("\nALLOC 11:\n");
	alloc_return_11 = VsaAlloc(new_vsa_2, 24);
	TESTS(alloc_return_11 != NULL);	
	TESTS(VsaLargestChunk(new_vsa_2) == 187);			
	
	printf("\nALLOC 12:\n");
	alloc_return_12 = VsaAlloc(new_vsa_2, 25);	
	TESTS(alloc_return_12 != NULL);	
	TESTS(VsaLargestChunk(new_vsa_2) == 131);
	
	printf("\nALLOC 13:\n");
	alloc_return_13 = VsaAlloc(new_vsa_2, 120);
	TESTS(alloc_return_13 != NULL);	
	TESTS(VsaLargestChunk(new_vsa_2) == 0);			
	
	printf("\nALLOC 14:\n");
	alloc_return_14 = VsaAlloc(new_vsa_2, 10);	
	TESTS(alloc_return_14 == NULL);	
	TESTS(VsaLargestChunk(new_vsa_2) == 0);		
	
	printf("\nFREE 12:\n");
	VsaFree(alloc_return_12);
	TESTS(VsaLargestChunk(new_vsa_2) == 32);					
	
	printf("\nFREE 10:\n");
	VsaFree(alloc_return_10);		
	TESTS(VsaLargestChunk(new_vsa_2) == 32);			
	
	printf("\nFREE 11:\n");
	VsaFree(alloc_return_11);	
	TESTS(VsaLargestChunk(new_vsa_2) == 120);		

	printf("\nFREE 13:\n");
	VsaFree(alloc_return_13);	
	TESTS(VsaLargestChunk(new_vsa_2) == 275);		
	
	free(new_ptr_2);
	
	return 0;
}
