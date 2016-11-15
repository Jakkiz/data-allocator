#include "block.h"

Block* allocate(Pool* pool, int size)
{
	int i = 0;
	int position = 0;
	int counterSize = pool->size;
	int offsetBlock = 0;
	Block* uallBlockReplace = NULL;
	Block* previousBlock = NULL;
	Block* next = NULL;
	if (size > 0 && size <= pool->size)
	{
		Block* newBlock = (Block*)malloc(sizeof(Block));
		previousBlock = retrieveLastBlock(pool->blockStorageArray);
		/*If it is the first block than I allocate it at the start of the pool*/
		if (previousBlock == NULL) 
		{
			offsetBlock = 0;
		}
		/*It means is not the first one that I allocate so I need to find the correct position*/
		else				       
		{
			/*If there are no empty spaces (unallocated blocks) I allocate next to the previous box */
			if (pool->ublockStorageArray[0] == NULL &&	size + previousBlock->offset + previousBlock->size <= pool->size)
			{
				offsetBlock = previousBlock->offset + previousBlock->size;
				previousBlock->nextBlock = newBlock;
			}
			else
			{
				/*I try to find the unBlocks that best fit my new block*/
				while (pool->ublockStorageArray[i] != NULL) 
				{
					if (pool->ublockStorageArray[i]->size > size && pool->ublockStorageArray[i]->size < counterSize)
					{
						counterSize = pool->ublockStorageArray[i]->size;
						uallBlockReplace = pool->ublockStorageArray[i];
						position = i;
					}
					i++;
				}
				/*Checks if I found an uallblock that can fit the newBlock*/
				if (uallBlockReplace != NULL)
				{
					next = previousBlock->nextBlock;
					previousBlock->nextBlock = newBlock;
					offsetBlock = uallBlockReplace->offset;
					uallBlockReplace->offset = uallBlockReplace->size - size;
					if (uallBlockReplace->size == 0)
					{
						pool->ublockStorageArray[position] = NULL;
						orderList(pool->ublockStorageArray);
					}
				}
				else
				{
					/*If we didnt find any unallocBlocks that can fit my newBlock I see if I can fit it at the end of pool*/
					if (size + previousBlock->offset + previousBlock->size <= pool->size) 
					{
						offsetBlock = previousBlock->offset + previousBlock->size;
						previousBlock->nextBlock = newBlock;
					}
					else
					{
						/*If I can't fit the new block at the end of pool I'll try to defrag in order to create more space*/
						defrag(pool);

						/*Checks if the space created after defrag is big enough to fit the newBlock*/
						if (size + previousBlock->offset + previousBlock->size <= pool->size)
						{
							previousBlock = retrieveLastBlock(pool->blockStorageArray);
							previousBlock->nextBlock = newBlock;
							offsetBlock = previousBlock->offset + previousBlock->size;
						}
						/*If the space is not enough I print out an Error an I return NULL*/
						else
						{
							printf("\tERROR STORING BLOCK, MEMORY IS FULL AFTER DEFRAG\n");
							return NULL;
						}
					}
				}
			}
		}
		/*Finally assigning the block to the corret place in pool memory*/
		newBlock->nextBlock = next;
		newBlock->size = size;
		newBlock->offset = offsetBlock;
		newBlock->poolPointer = pool;
		newBlock->memory = malloc(sizeof(char)*size);
		newBlock->poolPointer = pool;
		addToList(pool->blockStorageArray, newBlock); 
		return newBlock;
	}
	return NULL;
}

void write(Block* block, void *object, int objectSize, int offsetInBlock)
{
	if (block->size < objectSize || (offsetInBlock + objectSize) > block->size)
	{
		printf("\tERROR STORING DATA IN THE BLOCK\n");
		return;
	}
	memcpy((char*)block->memory + offsetInBlock, object, objectSize);
	return;
}

void* read(Block* block, int objectSize, int offsetInBlock)
{
	if (block->size < objectSize || (offsetInBlock + objectSize) > block->size)
	{
		printf("\tERROR RETRIEVING DATA FROM THE BLOCK\n");
		return NULL;
	}
	return (char*)block->memory + offsetInBlock;
}

void deallocate(Block* block)
{
	Block* previousBlock;
	Pool* pool;
	if (block != NULL && block->size >= 0)
	{
		pool = block->poolPointer;
		/*If there is only the first element in the allocated storage array I free it*/
		if (pool->blockStorageArray[1] == NULL)
		{
			free(block);
			pool->blockStorageArray[0] = NULL;
			return;
		}
		else
		{
			/*Remove block from allocated block array and moving it to the unallocated block array*/
			previousBlock = removeFromList(pool->blockStorageArray, block);				
			return;
		}
		return;
	}
}

/*Block Array Methods*/

/*Adds the block at the end of the specific array*/
void addToList(Block* storageArray[], Block* block)
{
	int success = 0;
	int i = 0;
	/*It loops until it finds an empty space in the array*/
	while (success == 0)
	{
		if (storageArray[i] != NULL)
		{
			i++;
		}
		/*It stores the reference to the block*/
		else
		{
			storageArray[i] = block;
			success = 1;
		}
	}
	return;
}

/*Remove the block from the specific list and it adds it to the unblockArray*/
Block* removeFromList(Block* storageArray[], Block* block)
{
	Block* previousBlock = NULL;
	Pool* pool;
	pool = block->poolPointer;
	int success = 0;
	int i = 0;
	/*It loops until it finds the block I'm looking for*/
	while (success == 0)
	{
		/*Checking if I found the block I'm looking for*/
		if (pool->blockStorageArray[i] == block)
		{
			/*If I find it, I link the previous block to the nextBlock of the block I want to delete*/
			previousBlock = pool->blockStorageArray[i-1];
			previousBlock->nextBlock = pool->blockStorageArray[i]->nextBlock;
			/*I add it to the unallocBlock array and set the old space where the block was to NULL*/
			addToList(pool->ublockStorageArray, block);
			pool->blockStorageArray[i] = NULL;
			/*I compact the list before going out of the loop*/
			orderList(pool->blockStorageArray);
			success = 1;
		}
		/*If I cant find the block I print out an error message*/
		else if (i > 25)
		{
			printf("\tError i > 25 cant find block\n");
			return previousBlock;
		}
		i++;
	}
	return previousBlock;
}

/*Retrieves last block from the specific storage array*/
Block* retrieveLastBlock(Block* storageArray[])
{
	Block* firstBlock = NULL;
	Block* blockReference = NULL;
	int max = 10000;
	int position = 0;
	int i = 0;
	/*I check if the storage array is not empty*/
	if (storageArray[0] != NULL)
	{
		/*I try to find the block with the smallest offset which is the first node of the list*/
		while (storageArray[i] != NULL)
		{
			if (storageArray[i]->offset < max)
			{
				max = storageArray[i]->offset;
				position = i;
			}
			i++;
		}
		/*Once I found the first node I loop through the list to get to the last node*/
		firstBlock = storageArray[position];
		while (firstBlock->nextBlock != NULL)
		{
			blockReference = firstBlock->nextBlock;
			firstBlock = blockReference;
		}
		/*Finally I return the last block in the list*/
		return firstBlock;
	}
	/*If the storage array is empty I return NULL*/
	else
	{
		return NULL;
	}
}

/*Compact the specific list bringing all the elements together (avoids having "holes" in the array)*/
void orderList(Block* storageArray[])
{
	int i = 0;
	int k = 1;
	int success = 0;
	/*While I dont find an empty space I keep reading the elements in the array*/
	while (storageArray[i] != NULL)
	{
		i++;
		k++;
	}
	/*If k and i are NULL means that the array has no empty spaces and I can return*/
	if (storageArray[k] == NULL && storageArray[i] == NULL)
	{
		return;
	}
	else
	{
		/*If I find the empty space I start moving the elements up to fill all the empty spaces*/
		while(storageArray[k] != NULL )
		{
			storageArray[i] = storageArray[k];
			storageArray[k] = NULL;
			i++;
			k++;
		}
	}
}

/*Moving all the blocks up to the begininning of the pool one after the other, trying to create more space for the new block*/
void defrag(Pool* pool)
{
	Block* nBlock = NULL;
	Block* blockReference = NULL;
	int smallerOffset = pool->size;
	int defragComplete = 0;
	int i = 0;
	/*I try to find the first block so I can use it to navigate the list of blocks*/
	while (pool->blockStorageArray[i] != NULL)
	{
		if (smallerOffset > pool->blockStorageArray[i]->offset)
		{
			smallerOffset = pool->blockStorageArray[i]->offset;
			blockReference = pool->blockStorageArray[i];
		}
		i++;
	}
	/*Moving first block in the pool back to the start */
	if (smallerOffset != 0)
	{
		blockReference->offset = 0;
	}
	/*Compacting all the blocks by changing the offset, moving them one after the other while looping through the blocks list*/
	nBlock = blockReference->nextBlock;
	while (nBlock != NULL)
	{
		nBlock->offset = blockReference->offset + blockReference->size;
		blockReference = nBlock;
		nBlock = blockReference->nextBlock;
	}
	printf("\tDefrag: Complete!\n");
	return;
}

/*Frees all the blocks in the pool*/
void clearAllBlocks(Pool* pool)
{
	int i = 0;
	/*Frees all the blocks in the allocated blocks array and it resets each element to NULL */
	while (pool->blockStorageArray[i] != NULL)
	{
		free(pool->blockStorageArray[i]);
		pool->blockStorageArray[i] = NULL;
		i++;
	}
	i = 0;
	/*Frees all the blocks in the unallocated blocks array and it resets each element to NULL */
	while (pool->ublockStorageArray[i] != NULL)
	{
		free(pool->ublockStorageArray[i]);
		pool->ublockStorageArray[i] = NULL;
		i++;
	}
	return;
}