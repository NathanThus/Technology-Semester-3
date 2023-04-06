#include "memory.h"
#include "linked_list.h"

#include <stdlib.h>

static const int StartAddress = 1000;

LinkedList* freeList = NULL;
LinkedList* allocatedList = NULL;

int MaxSize = 69;

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
    freeList = InitializeList(sizeof(MemoryBlock));
    allocatedList = InitializeList(sizeof(MemoryBlock));
    MemoryBlock block = {StartAddress, MaxSize};

    AddToListTail(freeList, &block);
    MemoryBlock* blck = RetrieveData(freeList, GetHead(freeList));
    blck = blck;
    return;
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

    fprintf(stream, "FreeList:\n");
    fprintf(stream, "---------\n");
    if(GetHead(freeList) == NULL)
    {
        fprintf(stream, " <empty>\n");
    }
    else
    {
        void* block = GetHead(freeList);
        int index = 0;
        while(block != NULL)
        {
            MemoryBlock* data = RetrieveData(freeList, block);
            if(data == NULL)
            {
                return -1;
            }
            fprintf(stream, " %d: addr:%d  size:%d\n",index, data->Address, data->Size);
            block = GetNext(freeList);
        }
    }

    fprintf(stream, "AllocatedList:\n");
    fprintf(stream, "----------\n");

        if(GetHead(allocatedList) == NULL)
    {
        fprintf(stream, " <empty>\n");
    }
    else
    {
        void* block = GetHead(allocatedList);
        int index = 0;
        while(block != NULL)
        {
            MemoryBlock* data = RetrieveData(allocatedList, block);
            if(data == NULL)
            {
                return -1;
            }
            fprintf(stream, " %d: addr:%d  size:%d\n",index, data->Address, data->Size);
            block = GetNext(allocatedList);
        }
    }
    // void* block = GetHead(freeList);
    // fprintf(stream, "[FREE]\n");
    // while(block != NULL)
    // {
    //     MemoryBlock* data = RetrieveData(freeList, block);
    //     if(data == NULL)
    //     {
    //         return -1;
    //     }
    //     fprintf(stream, "ADDR: %d: SIZE: %d\n", data->Address, data->Size);
    //     block = GetNext(freeList);
    // }

    // fprintf(stream, "[ALLOC]\n");
    // void* allocatedBlock = GetHead(allocatedList);
    // while(allocatedBlock != NULL)
    // {
    //     MemoryBlock* data = RetrieveData(allocatedList, allocatedBlock);
    //     if(data == NULL)
    //     {
    //         return -1;
    //     }
    //     fprintf(stream, "ADDR: %d: SIZE: %d\n", data->Address, data->Size);
    //     allocatedBlock = GetNext(allocatedList);
    // }

    return 0;
}

/* function: Insert
 * pre: -
 * post: if list is NULL, -1 is returned
 *       otherwise the block is inserted in the list and the start address of the block is returned
 */
int Insert(LinkedList* list, MemoryBlock block)
{
    if (list == NULL)
    {
        return -1;
    }
    
    int index = 0, finalindex = 0;
    void* movingPtr = GetHead(list);
    MemoryBlock* blockPtr = RetrieveData(list, movingPtr);

    while(movingPtr != NULL)
    {
        blockPtr = RetrieveData(list, movingPtr);
        if(blockPtr->Address > block.Address)
        {
            finalindex = index - 1;
        }
        index++;
        movingPtr = GetNext(list);
    }

    if(InsertAtIndex(list, &block, finalindex) == -1)
    {
        return -1;
    }

    return block.Address;
}

/* function: ClaimMemory
 * pre: nrofBytes > 0
 * post: if no memory block of nrofBytes available return -1
 *       otherwise the first block that is large enough is claimed and the start address is returned
 */
int ClaimMemory(int nrofBytes)
{
    void* movingPtr = GetHead(freeList);
    MemoryBlock* blockPtr = RetrieveData(freeList, movingPtr);
    int index = 0;
    while(movingPtr != NULL)
    {
        if(blockPtr->Size == nrofBytes) // Claim as is
        {
            AddToListTail(allocatedList,RetrieveData(freeList,movingPtr));
            RemoveDataFromList(freeList,index);
            return blockPtr->Address;
        }
        else if(blockPtr->Size > nrofBytes) // Split the block
        {
            MemoryBlock newAllocListBlock = {blockPtr->Address, nrofBytes}; 
            MemoryBlock nextFreeListBlock = {blockPtr->Address + nrofBytes, blockPtr->Size - nrofBytes};

            RemoveDataFromList(freeList, index);
            Insert(freeList, nextFreeListBlock);
            return Insert(allocatedList, newAllocListBlock);
        }
        movingPtr = GetNext(freeList);
        index++;
    }
    return -1;
}

/* function: Compress
 * pre: -
 * post: if no memory can be freed return -1
 *       otherwise return 0 if the memory was sucessfully compressed
 */
int Compress()
{
    return -1;
}

/* function: FreeMemory
 * pre: Parameter addr must match the start of an allocatd memory block. Otherwhise return -1.
 * post: Memory is freed and the number of freed bytes is returned
 *
 */
int FreeMemory(int addr)
{
    if(addr <= StartAddress || addr > StartAddress + MaxSize)
    {
        return -1;
    }

    MemoryBlock* block = GetHead(allocatedList);

    while(block != NULL)
    {
        if(block->Address == addr)
        {
            // Need to check if we can combine a few blocks.
            MemoryBlock* freeBlock = GetHead(allocatedList);
            while(freeBlock != NULL)
            {
                return -69;
            }
        }
        block = GetNext(allocatedList);
    }
    return -1;
}
