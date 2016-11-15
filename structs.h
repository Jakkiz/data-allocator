#ifndef __STRUCTS_H
#define __STRUCTS_H

typedef struct _BLOCK Block;
typedef struct _POOL Pool;

struct _POOL
{
	int size;
	void* memory;
	Block* blockStorageArray[25];
	Block* ublockStorageArray[25];
};

struct _BLOCK
{
	int size;
	int offset;
	void* memory;
	Block* nextBlock;
	Pool* poolPointer;
};
#endif