#ifndef MEM_MNG_H_
#define MEM_MNG_H_

/* Block size in bytes. */
#define BLOCK_SIZE			1
/* Number of blocks in static heap. */
#define BLOCKS_IN_HEAP		100

/* Allocate one block of data. */
void * mem_block_alloc(void);

/* Free one block of data. */
void mem_block_free(void * block);

#endif /* MEM_MNG_H_ */
