#include "memory.h"
#include "linked_list.h"

#include <stdlib.h>
#include <stdbool.h>

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

    fprintf(stream, "FreeList:\r\n");
    fprintf(stream, "---------\r\n");
    if(GetHead(freeList) == NULL)
    {
        fprintf(stream, " <empty>\r\n");
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
            fprintf(stream, "%4d:  addr:%d  size:%4d\r\n",index, data->Address, data->Size);
            block = GetNext(freeList);
            index++;
        }
    }

    // ALLOC

    fprintf(stream, "AllocList:\r\n");
    fprintf(stream, "----------\r\n");

    if(GetHead(allocatedList) == NULL)
    {
        fprintf(stream, " <empty>\r\n");
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
            fprintf(stream, "%4d:  addr:%d  size:%4d\r\n",index, data->Address, data->Size);
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
        MemoryBlock* movingBlock = RetrieveData(list, movingPtr);
        if(movingBlock->Address > block->Address)
        {
            finalindex = index;
            break;
        }
        movingPtr = GetNext(list);
        index++;
    }

    if(finalindex == -1)
    {
        AddToListTail(list, block);
    }
    else
    {
        InsertAtIndex(list, block, finalindex);
    }

    return block->Address;
}

/* function: ClaimMemory
 * pre: nrofBytes > 0
 * post: if no memory block of nrofBytes available return -1
 *       otherwise the first block that is large enough is claimed and the start address is returned
 */
int ClaimMemory(int nrofBytes)
{
    if(nrofBytes > MaxSize)
    {
        return -1;
    }
    
    void* movingPtr = GetHead(freeList);
    int index = 0;

    while(movingPtr != NULL)
    {
        MemoryBlock* blockPtr = RetrieveData(freeList, movingPtr);
        if(blockPtr->Size == nrofBytes) // Claim as is
        {
            MemoryBlock* block = RetrieveData(freeList,movingPtr);
            int address = block->Address;

            Insert(allocatedList, block);
            
            RemoveDataFromList(freeList,index);
            return address;
        }
        else if(blockPtr->Size > nrofBytes) // Split the block
        {
            MemoryBlock newAllocListBlock = {blockPtr->Address, nrofBytes}; 
            MemoryBlock nextFreeListBlock = {blockPtr->Address + nrofBytes, blockPtr->Size - nrofBytes};

            RemoveDataFromList(freeList, index);

            Insert(freeList, &nextFreeListBlock);
            
            int address = Insert(allocatedList, &newAllocListBlock);
            return address;
        }

        movingPtr = GetNext(freeList);
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
            int finalIndex = -1;
            void* freeBlock = GetHead(freeList);

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
            
            void* nextBlock = NULL;
            bool madeChange = true;
            while(madeChange)
            {
                madeChange = false;

                freeBlock = GetHead(freeList);
                if(freeBlock == NULL)
                {
                    return 0;
                }
                
                nextBlock = GetNext(freeList);
                if(nextBlock == NULL)
                {
                    return 0;
                }

                freeIndex = 1;

                while (nextBlock != NULL)
                {
                    MemoryBlock* freeBlockPtr = RetrieveData(freeList, freeBlock);
                    MemoryBlock* nextBlockPtr = RetrieveData(freeList, nextBlock);
                    if(freeBlockPtr == NULL || nextBlockPtr == NULL)
                    {
                        return 0;
                    }

                    if(freeBlockPtr->Address + freeBlockPtr->Size == nextBlockPtr->Address)
                    {
                        freeBlockPtr->Size += nextBlockPtr->Size;
                        RemoveDataFromList(freeList, freeIndex);
                        madeChange = true;
                    }

                    freeBlock = nextBlock;
                    nextBlock = GetNext(freeList);
                    freeIndex++;
                }
            }

            RemoveDataFromList(allocatedList, index);

            return bytes;
        }
        block = GetNext(allocatedList);
        index++;
    }
    return -1;
}
