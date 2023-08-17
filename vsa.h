#ifndef VSA_H
#define VSA_H

#include <stddef.h>	/* size_t */

typedef struct vsa vsa_t;

/* initializes a Variable Size Allocator (VSA) with the provided memory pool */
vsa_t *VsaInit(void *alloc_dest, size_t size);

/* allocates new memory block of the specified size within the VSA */
void *VsaAlloc(vsa_t *vsa, size_t block_size);

/* frees previously allocated memory block */
void VsaFree(void *block);

/* calculates size of largest contiguous free memory block in the VSA */
size_t VsaLargestChunk(vsa_t *vsa);

#endif /* VSA_H */

