#include "mem_mng.h"

#include <stdint.h>
#include <stddef.h>

/* User can redefine next macros for guarding
 * critical operations while allocating or freeing blocks. */
#ifndef __begin_atomic_process
	/* Called when there is need to begin atomic operation. */
    #define __begin_atomic_process()
#endif

#ifndef __end_atomic_process
	/* Called when atomic operation ends. */
    #define __end_atomic_process()
#endif

/* Block states holding in special heap map. */
typedef enum block_status_t {
	BLOCK_FREE,
	BLOCK_ALLOC
} block_status;

/* Static heap declaration and map of block statuses. */
#define HEAP_SIZE	(BLOCK_SIZE * BLOCKS_IN_HEAP)
static uint8_t mem_heap[HEAP_SIZE] = {0};
static block_status mem_blocks[BLOCKS_IN_HEAP] = {BLOCK_FREE};

void * mem_block_alloc(void) {
	uint8_t * alloc_block = NULL;
	uint16_t block;

	for (block = 0; block < BLOCKS_IN_HEAP; block++) {
		// Checkout block status and find first free block in heap.
		if (mem_blocks[block] == BLOCK_FREE) {

			// Allocating.
			__begin_atomic_process();
			mem_blocks[block] = BLOCK_ALLOC;
			alloc_block = &mem_heap[block * BLOCK_SIZE * sizeof(uint8_t)];
			__end_atomic_process();
			break;
		}
	}

	return (void *)alloc_block;
}

void mem_block_free(void * block) {
	uint8_t * alloc_block = (uint8_t *) block;

	if (alloc_block >= &mem_heap[0] && alloc_block < &mem_heap[BLOCK_SIZE * BLOCKS_IN_HEAP]) {

		// Calculate block position.
		uint16_t block_pos = (alloc_block - &mem_heap[0]) / BLOCK_SIZE;

		// Freeing.
		__begin_atomic_process();
		if (mem_blocks[block_pos] == BLOCK_ALLOC) {
			mem_blocks[block_pos] = BLOCK_FREE;
		}
		__end_atomic_process();
	}
}
