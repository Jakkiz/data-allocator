#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "pool.h"
#include "block.h"


int main()
{
	printf("****************************************\n");
	printf("*            Phase 2 Tests             *\n");
	printf("****************************************\n");
	int x = 5;
	char c = 'c';
	char str[] = "When life gives you lemons, make lemonade.";
	const int blockSize = 100;
	const int poolSize = 500;
	Pool* pool;
	Block* block;
	
	/* Test 1 - Allocate and deallocate a pool */
	printf("Test 1: Allocate and Deallocate a Block within a Pool\n");
	pool = allocatePool(50);
	block = allocate(pool, 50);
	if (block != NULL && pool != NULL)
	{
		printf("\tTest passed successfully: \n\tPool and Block allocated at: ");
		printf("%p", (void*)&pool);
		printf(" and ");
		printf("%p\n", (void*)&block);

	}
	else
	{
		printf("\tTest Failed: Pool and Block are equal to NULL");
	}
	deallocate(block);
	freePool(pool);
	pool = NULL;

	/* Tests 2-4 - Simple data storage*/
	pool = allocatePool(poolSize);
	block = allocate(pool, blockSize);
	
	printf("Test 2: Store a single character\n");
	write(block, &c, 1, 0);
	printf("\tStored: %c\n", c);
	printf("\tRetrieves: %c\n", *((char*)read(block,1,0)));

	printf("Test 3: Store a single multi-byte value\n");
	write(block, &x, 4, 3);
	printf("\tStored: %d\n", x);
	printf("\tRetrieves: %d\n", *((int*)read(block, 4, 3)));

	printf("Test 4: Store an arbitrary multi-byte value\n");
	write(block,str,strlen(str), 8);
	printf("\tStored: %s\n", str);
	printf("\tRetrieves: %s\n", (char*)read(block, strlen(str),8));

	/* Test 5 - Missing Null terminator */
	printf("Test 5: Store an arbitrary multi-byte value with no null terminator\n");
	write(block, str, strlen(str)-1, 8);
	printf("\tStored: %s\n", str);
	char* retstrb = (char*)read(block, strlen(str) - 1, 8);
	if (strlen(retstrb) > 12)
	{
		printf("\tPass: Correctly not null terminated %d\n", strlen(retstrb));
	}
	else
	{
		printf("\tFail: string is of incorrect length %d\n", strlen(retstrb));
	}

	/* Test 6 - Allocate past end of memory */
	block = allocate(pool, blockSize);
	printf("Test 6: Store / Retrieve past the end of memory\n");
	write(block, str, strlen(str) , blockSize-1);
	printf("\tStored: %s\n", str);
	char* tempb = (char*)read(block, strlen(str) , blockSize-1);
	if (tempb != NULL)
	{
		printf("\tFail: Retrieved a value\n");
	}
	else
	{
		printf("\tPass: Retrieved NULL\n");
	}
	deallocate(block);

	/* Test 7 - Double Deallocate Pool */
	printf("Test 7: Double Deallocate Block\n");
	deallocate(block);
	printf("\tPass\n");

	printf("Test 8: Deallocate NULL Block\n");
	block = NULL;
	deallocate(block);
	printf("\tPass\n");

	/* Test 9 - Allocate 0 Block */
	printf("Test 9: Allocate Empty Pool\n");
	block = allocate(pool,0);
	if (block == NULL || block->size == 0)
	{
		printf("\tPass: Block is NULL, or Block->size is 0\n");
	}
	else
	{
		printf("\tFail: Block exists\n");
	}
	deallocate(block);
	block = NULL;

	/* Test 10 - Allocate negative Block */
	printf("Test 10: Allocate negative Block\n");
	block = allocate(pool, -blockSize);
	if (block == NULL)
	{
		printf("\tPass: Block is NULL\n");
	}
	else
	{
		printf("\tFail: Black is not NULL\n");
	}
	deallocate(block);
	freePool(pool);

	/* Test 11 - Allocate Multiple Blocks */
	printf("Test 11: Allocate Multiple Block\n");
	pool = allocatePool(poolSize);
	block = allocate(pool, blockSize);
	Block* microBlock1 = allocate(pool, 25);
	Block* microBlock2 = allocate(pool, 25);
	Block* microBlock3 = allocate(pool, 25);
	Block* microBlock4 = allocate(pool, 25);
	Block* macroBlock1 = allocate(pool, blockSize);
	Block* macroBlock2 = allocate(pool, blockSize);
	Block* macroBlock3 = allocate(pool, blockSize);

	deallocate(microBlock1);
	deallocate(microBlock2);

	Block* failBlock = allocate(pool, blockSize);
	if (failBlock == NULL)
	{
		printf("\tPass: Block is NULL\n");
	}
	else
	{
		printf("\tFail: Black is not NULL\n");
	}
	deallocate(microBlock3);
	deallocate(microBlock4);

	/* Test 12 - Contiguous Space Test*/
	printf("Test 12: Contiguous Space Test\n");
	Block* passBlock = allocate(pool, blockSize);
	if (passBlock)
	{
		printf("\tPass: Block is not NULL, contiguous space found\n");
	}
	else
	{
		printf("\tFail: Black is NULL, contiguous space not reclaimed\n");
	}
	deallocate(block);
	deallocate(passBlock);
	deallocate(macroBlock1);
	deallocate(macroBlock2);
	deallocate(macroBlock3);

	
	microBlock1 = allocate(pool, 25);
	block = allocate(pool, blockSize);
	microBlock2 = allocate(pool, 25);
	macroBlock1 = allocate(pool, blockSize);
	macroBlock2 = allocate(pool, blockSize);
	microBlock3 = allocate(pool, 25);
	macroBlock3 = allocate(pool, blockSize);
	microBlock4 = allocate(pool, 25);

	deallocate(microBlock1);
	deallocate(microBlock2);
	deallocate(microBlock3);
	deallocate(microBlock4);

	/* Test 13 - Fragmentation Space Test*/
	printf("Test 13: Fragmentation Space Test\n");
	passBlock = allocate(pool, blockSize);
	if (passBlock)
	{
		printf("\tPass: Block is not NULL, Defragmented space found\n");
	}
	else
	{
		printf("\tFail: Black is NULL, Fragmented space not reclaimed\n");
	}
	
	deallocate(block);
	deallocate(macroBlock1);
	deallocate(macroBlock2);
	deallocate(macroBlock3);
	deallocate(passBlock);

	//My Tests
	printf("****************************************\n");
	printf("*        Supplementary Tests           *\n");
	printf("****************************************\n");

	/* Test 14 - Allocate Block Best fit */
	printf("Test 14: Allocate a Block Best Fit\n");
	block = allocate(pool, blockSize);
	microBlock1 = allocate(pool, 25);
	microBlock2 = allocate(pool, 25);
	microBlock3 = allocate(pool, 25);
	microBlock4 = allocate(pool, 25);
	macroBlock1 = allocate(pool, blockSize);
	macroBlock2 = allocate(pool, blockSize);
	macroBlock3 = allocate(pool, blockSize);
	printf("\tAllocated Blocks of size: |100|25|25|25|25|100|100|100|\n");

	//Creating Holes in the pool of size 25 - 25 - 25 - 100 
	deallocate(microBlock1);
	deallocate(microBlock2);
	deallocate(microBlock3);
	deallocate(macroBlock1);
	printf("\tCreating Holes in the pool: |100|--|--|--|25|--|100|100|\n");

	//Trying to allocate a Block size 75
	printf("\tTrying to allocate a Block size: 75\n");
	passBlock = allocate(pool, 75);
	if (passBlock != NULL)
	{
		printf("\tPass: Block is not NULL, Block size 75 it's been allocated\n");
	}
	else
	{
		printf("\tFail: Block is NULL\n");
	}

	//Trying to allocate a Block size 100
	printf("\tTrying to allocate a Block size: 100\n");
	failBlock = allocate(pool, 100);
	if (failBlock != NULL)
	{
		printf("\tPass: Block is not NULL, Block size 100 it's been allocated \n\t      without defragmentation!\n");
	}
	else
	{
		printf("\tFail: Block is NULL\n");
	}
	deallocate(microBlock4);
	deallocate(block);
	deallocate(macroBlock2);
	deallocate(macroBlock3);
	deallocate(passBlock);
	deallocate(failBlock);
	freePool(pool);

	/* Test 15 - Testing Defragmentation */
	printf("Test 15: Testing Defragmentation\n");
	pool = allocatePool(poolSize);
	block = allocate(pool, blockSize);
	microBlock1 = allocate(pool, 25);
	microBlock2 = allocate(pool, 25);
	microBlock3 = allocate(pool, 25);
	microBlock4 = allocate(pool, 25);
	macroBlock1 = allocate(pool, blockSize);
	macroBlock2 = allocate(pool, blockSize);
	macroBlock3 = allocate(pool, blockSize);	
	printf("\tAllocated Blocks of size: |100|25|25|25|25|100|100|100|\n");

	//Creating Holes in the pool of size 25 - 25 - 100 - 100
	printf("\tCreating Holes in the pool: |100|--|25|--|25|--|100|--|\n");
	deallocate(microBlock1);
	deallocate(microBlock3);
	deallocate(macroBlock1);
	deallocate(macroBlock3);

	//Trying to allocate a Block size 250
	printf("\tTrying to allocate a Block size: 250\n");
	passBlock = allocate(pool, 250);
	if (passBlock != NULL)
	{
		printf("\tPass: Block is not NULL, Block it's been allocated after defrag\n");
	}
	else
	{
		printf("\tFail: Block is NULL\n");
	}
	deallocate(microBlock4);
	deallocate(block);
	deallocate(macroBlock2);
	deallocate(passBlock);

	clearAllBlocks(pool);
	freePool(pool);

	return 0;
}
