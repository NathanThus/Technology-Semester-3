#include "memory.h"
#include "linked_list.h"
#include <stdlib.h>
#include <stdbool.h>

static const int StartAddress = 1000;

int MaxMemory = 0;
int TotalClaimedMemory;

LinkedList* Free = NULL;
LinkedList* Allocated = NULL;

typedef struct 
{
    int Address;
    int Bytes;
}MemoryBlock;


/* function: ConstructMemory
 * pre: -
 * post: memory administration is constructed
 */
void ConstructMemory(int size)
{
    Free = InitializeList(size);
    Allocated = InitializeList(size);
}

/* function: DestructMemory
 * pre: -
 * post: memory administration is destructed
 */
void DestructMemory()
{
    DestructList(&Free);
    DestructList(&Allocated);
}


/* function: PrintList
 * pre: -
 * post: if stream is valid, list information is printed to stream and 0 is returned
 *       if stream is NULL, -1 is returned
 */
int PrintList(FILE* stream)
{
    if (stream == NULL)
    {
        return -1;
    }

    printf("Free list:\n---------\n");
    if(TotalClaimedMemory == MaxMemory)
    {
        printf(" <empty>\n");
    }
    else
    {
        MemoryBlock* elementPtr = (MemoryBlock*)GetHead(Free);
        while (elementPtr != NULL)
        {
            fprintf(stream,"Address: %d, Bytes: %d \n", elementPtr->Address, elementPtr->Bytes);
            elementPtr = (MemoryBlock*)GetNext(Free); 
        }
    }

    printf("Alloc list:\n---------\n");
    if(TotalClaimedMemory == 0)
    {
        printf(" <empty>\n");
    }
    else
    {
        MemoryBlock* elementPtr = (MemoryBlock*)GetHead(Free);
        while (elementPtr != NULL)
        {
            fprintf(stream,"Address: %d, Bytes: %d \n", elementPtr->Address, elementPtr->Bytes);
            elementPtr = (MemoryBlock*)GetNext(Free); 
        }
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
    if (nrofBytes <= 0 || TotalClaimedMemory + nrofBytes > MaxMemory)
    {
        return -1;
    }

    MemoryBlock* FreePtr = (MemoryBlock*)GetHead(Free);
    while(FreePtr != NULL)
    {
        //Check if the available block is large enough
        if(FreePtr->Bytes >= nrofBytes)
        {
            //Check if the available block is exactly the size we need
            if(FreePtr->Bytes == nrofBytes)
            {
                //Remove the block from the free list
                RemoveDataFromList(Free, FreePtr);
            }
            else
            {
                //Update the block in the free list
                FreePtr->Address += nrofBytes;
                FreePtr->Bytes -= nrofBytes;
            }

            //Add the block to the allocated list
            MemoryBlock* AllocatedPtr = (MemoryBlock*)malloc(sizeof(MemoryBlock));
            AllocatedPtr->Address = FreePtr->Address;
            AllocatedPtr->Bytes = nrofBytes;
            AddToListTail(Allocated, AllocatedPtr);

            TotalClaimedMemory += nrofBytes;
            return AllocatedPtr->Address;
        }

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
    if (addr < StartAddress)
    {
        return -1;
    }

    MemoryBlock* AllocatedPtr = (MemoryBlock*)GetHead(Allocated);
    while(AllocatedPtr != NULL)
    {
        //Check if the allocated block is the one we want to free
        if(AllocatedPtr->Address == addr)
        {
            //Remove the block from the allocated list
            RemoveDataFromList(Allocated, AllocatedPtr);

            //Add the block to the free list
            MemoryBlock* FreePtr = (MemoryBlock*)malloc(sizeof(MemoryBlock));
            FreePtr->Address = AllocatedPtr->Address;
            FreePtr->Bytes = AllocatedPtr->Bytes;
            AddToListTail(Free, FreePtr);

            TotalClaimedMemory -= AllocatedPtr->Bytes;
            return AllocatedPtr->Bytes;
        }
    }

    return 0;
}
