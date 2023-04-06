#include "memory.h"
#include "linked_list.h"

#include <stdlib.h>

static const int StartAddress = 1000;

LinkedList* freeList = NULL;
LinkedList* allocatedList = NULL;

int MaxSize = 69; // This is overwritten in ConstructMemory

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
    DestructList(&freeList);
    DestructList(&allocatedList);
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
            fprintf(stream, "%3d:  addr:%d  size:%4d\n",index, data->Address, data->Size);
            block = GetNext(freeList);
            index++;
        }
    }

    // ALLOC

    fprintf(stream, "AllocList:\n");
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
            fprintf(stream, "%3d:  addr:%d  size:%4d\n",index, data->Address, data->Size);
            block = GetNext(allocatedList);
            index++;
        }
    }

    return 0;
}

/* function: Insert
 * pre: -
 * post: if list is NULL, -1 is returned
 *       otherwise the block is inserted in the list and the start address of the block is returned
 */
int Insert(LinkedList* list, MemoryBlock* block)
{
    if (list == NULL)
    {
        return -1;
    }
    
    int index = 0, finalindex = -1;
    void* movingPtr = GetHead(list);

    while(movingPtr != NULL)
    {
        MemoryBlock* DataInListPtr = RetrieveData(list, movingPtr);
        if(DataInListPtr->Address > block->Address)
        {
            finalindex = index;
            break;
        }
        index++;
        movingPtr = GetNext(list);
    }

    if(finalindex == -1)
    {
        AddToListTail(list, block);
        return block->Address;
    }
    else
    {
        if(InsertAtIndex(list, block, finalindex) == -1)
        {   
            return -1;
        }
        return block->Address;
    }


}

/* function: ClaimMemory
 * pre: nrofBytes > 0
 * post: if no memory block of nrofBytes available return -1
 *       otherwise the first block that is large enough is claimed and the start address is returned
 */
int ClaimMemory(int nrofBytes)
{
    if(nrofBytes <= 0)
    {
        return -1;
    }

    if(nrofBytes > MaxSize)
    {
        printf("[ALLOC] not enough memory for %d byte", nrofBytes);
        return -1;
    }
    
    void* movingPtr = GetHead(freeList);
    MemoryBlock* blockPtr = RetrieveData(freeList, movingPtr);
    int index = 0;

    while(movingPtr != NULL)
    {
        if(blockPtr->Size == nrofBytes) // Claim as is
        {
            MemoryBlock* block = RetrieveData(freeList,movingPtr);
            Insert(allocatedList, block);
            RemoveDataFromList(freeList,index);
            return blockPtr->Address;
        }
        else if(blockPtr->Size > nrofBytes) // Split the block
        {
            MemoryBlock newAllocListBlock = {blockPtr->Address, nrofBytes}; 
            MemoryBlock nextFreeListBlock = {blockPtr->Address + nrofBytes, blockPtr->Size - nrofBytes};

            RemoveDataFromList(freeList, index);
            Insert(freeList, &nextFreeListBlock);
            return Insert(allocatedList, &newAllocListBlock);
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
int Compress(MemoryBlock* keepBlock, MemoryBlock* blockToBeCompressed)
{
    if(keepBlock == NULL || blockToBeCompressed == NULL)
    {
        return -1;
    }
    keepBlock->Size += blockToBeCompressed->Size;
    return 0;
}


/* function: FreeMemory
 * pre: Parameter addr must match the start of an allocatd memory block. Otherwhise return -1.
 * post: Memory is freed and the number of freed bytes is returned
 *
 */
int FreeMemory(int addr)
{
    if(addr >= MaxSize + StartAddress || addr < StartAddress)
    {
        return -1;
    }

    void* block = GetHead(allocatedList);
    int index = 0;
    while(block != NULL)
    {
        MemoryBlock* blockPtr = RetrieveData(allocatedList, block);
        if(blockPtr->Address == addr)
        {
            int bytes = blockPtr->Size;
            int freeIndex = 0;
            int finalIndex = -1; // Make sure that there's a save point
            void* freeBlock = GetHead(freeList); // ElmentPtr

            // Find where we need to insert it into the list, in the ascending order
            while(freeBlock != NULL)
            {
                MemoryBlock* freeBlockPtr = RetrieveData(freeList, freeBlock);
                if(freeBlockPtr->Address > addr)
                {
                    finalIndex = freeIndex;

                    break;
                }
                freeIndex++;
                freeBlock = GetNext(freeList);
            }

            // If we're at the end of the list, just add it to the end
            if(finalIndex == -1)
            {
                AddToListTail(freeList, blockPtr);
            }
            else
            {
                InsertAtIndex(freeList, blockPtr, finalIndex);
            }

            // Check if we can compress the memory
            if(Compress(blockPtr, (MemoryBlock*)RetrieveData(freeList,GetLastAccessed(freeList))) == -1)
            {
                return -1;
            }

            // Remove the block from the allocated list
            RemoveDataFromList(allocatedList, index);

            return bytes;
        }
        block = GetNext(allocatedList);
        index++;
    }
    return -1;
}
