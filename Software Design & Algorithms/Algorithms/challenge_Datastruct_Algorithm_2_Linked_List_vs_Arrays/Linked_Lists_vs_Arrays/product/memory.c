#include "memory.h"
#include "linked_list.h"

#include <stdlib.h>

static const int StartAddress = 1000;

LinkedList* freeList = NULL;
LinkedList* allocatedList = NULL;

int MaxSize = 0;

typedef struct  
{
    int Address;
    int Size;
}MemoryBlock;

/* function: ConstructMemory
 * pre: -
 * post: memory administration is constructed
 */
void ConstructMemory(int size)
{
    MaxSize = size;
    freeList = InitializeList(sizeof(int));
    allocatedList = InitializeList(sizeof(int));
    MemoryBlock block = {StartAddress, size};

    AddToListTail(freeList, &block);
}

/* function: DestructMemory
 * pre: -
 * post: memory administration is destructed
 */
void DestructMemory()
{
    free(freeList);
    free(allocatedList);
}


/* function: PrintList
 * pre: -
 * post: if stream is valid, list information is printed to stream and 0 is returned
 *       if stream is NULL, -1 is returned
 */
int PrintList(FILE* stream)
{
    if(stream == NULL)
    {
        return -1;
    }

    void* block = GetHead(freeList);
    fprintf(stream, "[FREE]\n");
    while(block != NULL)
    {
        MemoryBlock* data = RetrieveData(freeList, block);
        if(data == NULL)
        {
            return -1;
        }
        fprintf(stream, "ADDR: %d: SIZE: %d\n", data->Address, data->Size);
        block = GetNext(freeList);
    }

    fprintf(stream, "[ALLOC]\n");
    void* allocatedBlock = GetHead(allocatedList);
    while(allocatedBlock != NULL)
    {
        MemoryBlock* data = RetrieveData(allocatedList, allocatedBlock);
        if(data == NULL)
        {
            return -1;
        }
        fprintf(stream, "ADDR: %d: SIZE: %d\n", data->Address, data->Size);
        allocatedBlock = GetNext(allocatedList);
    }

    return 0;
}

/* function: ClaimMemory
 * pre: nrofBytes > 0
 * post: if no memory block of nrofBytes available return -1
 *       otherwise the first block that is large enough is claimed and the start address is returned
 */
int ClaimMemory(int nrofBytes)
{
    MemoryBlock* block = GetHead(freeList);
    int index = 0;
    while(block != NULL)
    {
        if(block->Size == nrofBytes) // Claim as is
        {
            AddToListTail(allocatedList,RetrieveData(freeList,block));
            RemoveDataFromList(freeList,index);
        }
        else if(block->Size > nrofBytes) // Split the block
        {
            // MemoryBlock newFreeListBlock = {block->Address + nrofBytes, block->Size - nrofBytes};
            // MemoryBlock newAllocListBlock = {block->Address,nrofBytes};
            
            // Now to make sure the free list block gets yeeted into the right location.

        }
        GetNext(freeList);
        index++;
    }
    return -1;
}

/* function: FreeMemory
 * pre: Parameter addr must match the start of an allocatd memory block. Otherwhise return -1.
 * post: Memory is freed and the number of freed bytes is returned
 *
 */
int FreeMemory(int addr)
{
    MemoryBlock* block = GetHead(allocatedList);

    while(block != NULL)
    {
        if(block->Address == addr)
        {
            // Need to check if we can combine a few blocks.
            MemoryBlock* freeBlock = GetHead(freeList);
            while(freeBlock != NULL)
            {
                if(freeBlock->Address + freeBlock->Size == block->Address)
                {
                    freeBlock->Size += block->Size;
                    RemoveDataFromList(allocatedList, 0);
                    return block->Size;
                }
                else if(block->Address + block->Size == freeBlock->Address)
                {
                    freeBlock->Address = block->Address;
                    freeBlock->Size += block->Size;
                    RemoveDataFromList(allocatedList, 0);
                    return block->Size;
                }
                freeBlock = GetNext(freeList);
            }
        }
        block = GetNext(allocatedList);
    }
    return -1;
}
