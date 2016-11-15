#include "pool.h"

/*It allocates a new pool*/
Pool* allocatePool(int n)
{
	Pool* newPool = NULL;
	int i = 0;
	if (n > 0)
	{
		newPool = (Pool*)malloc(sizeof(Pool));
		newPool->size = n;
		/*Initializing the two arrays setting all the elements to NULL*/
		for (i = 0; i < 25; i++)
		{
			newPool->blockStorageArray[i] = NULL;
		}
		for (i = 0; i < 25; i++)
		{
			newPool->ublockStorageArray[i] = NULL;
		}
		newPool->memory = malloc(sizeof(char)*n);
	}
	return newPool;
}

/*Freeing the pool*/
void freePool(Pool* pool)
{
	/*If the pool has a value than I free it*/
	if (pool != NULL)
	{
		free(pool->memory);
		free(pool);
	}
	return;
}

/*Storing object in the pool*/
void store(Pool* pool, int offset, int size, void *object)
{
	if (pool->size < offset || (offset + size) > pool->size)
	{
		printf("ERROR STORING THE POOL\n");
		return;
	}
	memmove((char*)pool->memory + offset, object, size);
	return;
}

/*Retrieving an object from the pool*/
void *retrieve(Pool* pool, int offset, int size)
{
	if (pool->size < offset || (offset + size) > pool->size)
	{
		printf("ERROR RETRIEVING THE POOL\n");
		return NULL;
	}
	return (char*)pool->memory + offset;
}