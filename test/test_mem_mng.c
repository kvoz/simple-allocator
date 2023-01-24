#ifdef TEST

#include "unity.h"

#include "mem_mng.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_simpleAllocStoreAndRetrieve(void)
{
	uint8_t * tmp;
	tmp = mem_block_alloc();
	TEST_ASSERT_NOT_NULL_MESSAGE(tmp, "NULL pointer after allocation");

	*tmp = 1;
	TEST_ASSERT_EQUAL_INT_MESSAGE(1, *tmp, "Stored value not valid");

	mem_block_free(tmp);
}

void test_whenAllocatedFewBlocks_thenAllocatedAddressesNotCrossing(void)
{
	uint8_t * tmp1, * tmp2;

	// Alloc first block
	tmp1 = mem_block_alloc();
	*tmp1 = 1;

	// Alloc second block
	tmp2 = mem_block_alloc();
	*tmp2 = 2;

	TEST_ASSERT_NOT_EQUAL_INT_MESSAGE(tmp1, tmp2, "Address of allocated blocks are equals");

	mem_block_free(tmp1);
	mem_block_free(tmp2);
}

void test_whenAllocatedFewBlocksAndFreeFirst_thenNewAllocatedBlockWillReuseIt(void)
{
	uint8_t * tmp1, * tmp2, * tmp3;

	// Alloc first block
	tmp1 = mem_block_alloc();
	*tmp1 = 1;

	// Alloc second block
	tmp2 = mem_block_alloc();
	*tmp2 = 2;

	// Free first block
	mem_block_free(tmp1);

	// Alloc third block
	tmp3 = mem_block_alloc();
	*tmp3 = 3;

	TEST_ASSERT_EQUAL_INT_MESSAGE(tmp1, tmp3, "Memory block nor reused after freeing");

	mem_block_free(tmp2);
	mem_block_free(tmp3);
}

void test_whenAllocatedMoreThanMaxBlocks_thenAllocatorNotCauseMemoryLeak(void)
{
	uint8_t * tmp[BLOCKS_IN_HEAP + 1];

	for(uint32_t i = 0; i < BLOCKS_IN_HEAP + 1; i++) {
		tmp[i] = mem_block_alloc();
	}

	TEST_ASSERT_NULL_MESSAGE(tmp[BLOCKS_IN_HEAP], "There is memory leak after max block number");

	for(uint32_t i = 0; i < BLOCKS_IN_HEAP; i++) {
		mem_block_free(tmp[i]);
	}
}

#endif // TEST
