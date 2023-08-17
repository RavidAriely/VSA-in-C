#include <stdio.h>
#include <assert.h>       /* assert */
#include "vsa.h"

#define WORD_SIZE sizeof(void*)
#define KEY 0xDEADBEEF
 
typedef struct header header_t;

/*
 * Struct:  header 
 * --------------------
 *   header holds metadata about a memory block within the VSA
 *
 *   is_free:       1 for free, 0 for occupied 
 *   key:	     for debugging purposes
 *   next_block:    pointer to the next memory block
 *   block_size:    size of memory block
 */
struct header
{
	int is_free;	
	int key;
	void *next_block;
	size_t block_size;
};

/*
 * Struct:  vsa 
 * --------------------
 *   contains metadata for the entire allocator
 *
 *   count_headers: count of headers
 *   first_header:  pointer to the first header
 */
struct vsa
{
	size_t count_headers;
	size_t* first_header;
};

/*
 * Function:  VsaInit 
 * --------------------
 *   recieves allocated memory address and size of the memory pool
 *   aligns the address to the machine word size, if needed
 *   initializes VSA struct and header struct for the first memory block
 *
 *   alloc_dest: pointer to allocated memory address for the VSA
 *   size:	 total size of the memory pool available for the VSA
 *
 *   returns: pointer to aligned address of the VSA (management struct)
 */
vsa_t *VsaInit(void *alloc_dest, size_t size)
{
	size_t remainder = 0;                                            
	size_t new_size = size;
	char *first_address = NULL;
	
	vsa_t *new_vsa = NULL;                                                                                                  
	header_t *new_header = NULL; 
	
	assert(alloc_dest);
	
	first_address = (char*)alloc_dest;   
	remainder = (size_t)alloc_dest % WORD_SIZE;                                                    
	
	assert(size >= (remainder + sizeof(vsa_t) + sizeof(header_t)));                                       
	
	if(0 != remainder)                
	{
		first_address += (WORD_SIZE - remainder);                                             
		new_size -= (WORD_SIZE - remainder);                                                   
	}
	
	new_vsa = (vsa_t*)first_address;                                                               
	new_vsa -> count_headers = 1;
	new_vsa -> first_header = (size_t*)first_address + (sizeof(vsa_t)/WORD_SIZE);
	
	new_header = (header_t*)new_vsa -> first_header;                                               
	new_header -> is_free = 1;                                                                    
	new_header -> key = KEY;                                                                       
	new_header -> next_block = NULL;      
	/* assign block size to size of memory pool minus header size*/                                                        
	new_header -> block_size = new_size - sizeof(vsa_t);                                          
	
	return new_vsa;
}

/*
 * Function:  Defragmentation 
 * --------------------
 *   iterates through the VSA headers, checks for consecutive free memory blocks, and merges them into a single larger free block.
 *
 *   vsa: pointer to the VSA to be defragmented
 *
 *   returns: doesn't have return value
 */
static void Defragmentation(vsa_t *vsa)
{
	header_t *current_header = NULL;                                                                                                 
	header_t *next_header = NULL;                                                                  
	size_t iterator_headers = 0;                                                                   
	
	assert(vsa);
	 
	current_header = (header_t*)vsa -> first_header;                                 
	next_header = current_header -> next_block;                                              
	iterator_headers = vsa -> count_headers;                                             
	 
	while((1 < iterator_headers))                                                                                              
	{
		if((current_header -> is_free == 1) && (next_header-> is_free == 1))                 
		{
			current_header -> block_size += next_header-> block_size;                
			current_header -> next_block = next_header -> next_block;                   
			next_header -> key = 0;                                              
			next_header = next_header -> next_block;                                     
			
			vsa -> count_headers--;                                              
		}
		
		else                                                                           
		{	
			current_header = current_header -> next_block;                             
			next_header = next_header -> next_block;
		}
		
		iterator_headers--;	                                                              
	}
}

/*
 * Function:  VsaLargestChunk 
 * --------------------
 *   uses Defragmentation function to ensure memory blocks are merged
 *   iterates through the given VSA headers to find the largest contiguous free memory chunk
 *
 *   vsa: pointer to the VSA to be analyzed
 *
 *   returns: size of largest free memory chunk
 */
size_t VsaLargestChunk(vsa_t *vsa)
{
	header_t *header_iterator = NULL;                                                              
	size_t return_size = 0;                                                                 
	size_t iterator_headers = 0;                                                              
                                                                      
	assert(vsa);
	
	header_iterator = (header_t*)vsa -> first_header;
	iterator_headers = vsa -> count_headers;
	
	Defragmentation(vsa);
	
	iterator_headers = vsa -> count_headers;                                                     
	
	while(0 < iterator_headers)                                                                  
	{
		if((header_iterator -> is_free == 1) && ((header_iterator -> block_size) > return_size)) 
		{
			return_size = header_iterator -> block_size - (sizeof(header_t));            
		}
		
		header_iterator = header_iterator -> next_block;                                       
		iterator_headers--;	                                                               
	}
	
	return return_size;
}




/*
 * Function:  AllocateBlock 
 * --------------------
 *   used by the VsaAlloc function to allocate a memory block and create a new free memory block when the available free block is larger than the requested size
 *
 *   vsa: 		pointer to the VSA       
 *   header_iterator:  pointer to header of original free block
 *   prev_block_size:  size of original free block
 *   new_block_size:   size of memory block to allocate
 *   saved_next:       pointer to next block after the original free block
 *
 *   returns: doesn't have return value
 */
static void *AllocateBlock(vsa_t *vsa, header_t *header_iterator, size_t prev_block_size, size_t new_block_size, void *saved_next)
{
	void *return_block = (size_t*)header_iterator + (sizeof(header_t)/WORD_SIZE);        
	saved_next = header_iterator -> next_block;                                   
	header_iterator -> is_free = 0;                                               
	header_iterator -> key = KEY;                                                                                              
	
	if(prev_block_size - new_block_size >= (sizeof(header_t) + WORD_SIZE))         
	{
		header_iterator -> next_block = (char*)header_iterator + new_block_size; 
		header_iterator -> block_size = new_block_size;                      
					
		header_iterator = header_iterator -> next_block;                       
		header_iterator -> is_free = 1;                                             
		header_iterator -> block_size = prev_block_size - new_block_size;      
		header_iterator -> key = KEY;                                         
					
		if(NULL == saved_next)                                           
		{
			header_iterator -> next_block = NULL;                         
		}
		else
		{
			header_iterator -> next_block = saved_next;                  
		}
					
		vsa -> count_headers++;                                        
	}
	
	return return_block;
}

/*
 * Function:  VsaAlloc 
 * --------------------
 *   uses Defragmentation function to ensure memory blocks are merged
 *   searches for a free memory block in the VSA for the requested size (plus header size)
 *   if the size of block is larger than required, it splits the block into two blocks, and uses the first block for the allocation
 *
 *   vsa:          pointer to the VSA where the new block will be allocated
 *   block_size:   size of memory block to be allocated
 *
 *   returns: pointer to beginning of allocated memory block or NULL if allocation fails
 */
void *VsaAlloc(vsa_t *vsa, size_t block_size)
{
	header_t *header_iterator = NULL;                                                             
	void *saved_next = NULL;                                                                      
	void *return_block = NULL;                                                             
	size_t remainder = block_size % WORD_SIZE;                                             
	size_t new_block_size = block_size + sizeof(header_t);                                      
	size_t iterator_headers = 0;                                                                  
	size_t prev_block_size = 0;                                                                  
	
	assert(vsa);
	
	header_iterator = (header_t*)vsa -> first_header;                                            
	iterator_headers = vsa -> count_headers;                                                   

	if(0 != remainder)
	{
		new_block_size += (WORD_SIZE - remainder);                                         
	}
	
	Defragmentation(vsa);
	
	while(0 < iterator_headers)
	{
		prev_block_size = header_iterator -> block_size;                                     
		
		if((header_iterator -> is_free == 1) && (header_iterator -> block_size >= new_block_size))   
		{
			return_block = AllocateBlock(vsa, header_iterator, prev_block_size, new_block_size, saved_next);
			break;
		}
		
		header_iterator = header_iterator -> next_block;                                   
		iterator_headers--;                                                                                 
	}
	
	return return_block;
}


/*
 * Function:  VsaFree 
 * --------------------
 *   frees a previously allocated memory block in the VSA for future re-allocation
 *
 *   block: pointer to beginning of allocated memory block to free
 *
 *   returns: doesn't have return value
 */
void VsaFree(void *block)
{
	header_t *block_header = 0;                                                               
	
	assert(block);
	
	block_header = (header_t *)block - 1;                                                        
	
	#ifdef DEBUG
		assert(block_header -> key == KEY);                                                  
	#endif
	
	block_header -> is_free = 1;                                                              
}








