#include "linked_list.h"

#include <stdlib.h>

typedef struct element Element;
struct element
{
    void* Data;
    Element* Next;
};

struct linkedList
{
    int ObjectSize;
    Element* Head;
    Element* LastAccessed;
};


LinkedList* InitializeList(int objectSize)
{
    if(objectSize < 1)
    {
        return NULL;
    }
    LinkedList *this = malloc(sizeof(LinkedList));
    this->ObjectSize = objectSize;
    this->Head = NULL;
    return this;
}

int AddToListTail(LinkedList* list, void* data)
{
    if (data == NULL || list == NULL)
    {
        return -1;
    }

    if (list->Head == NULL)
    {
        list->Head = malloc(sizeof(Element));
        list->Head->Data = malloc(list->ObjectSize);
        list->Head->Data = data;
        list->Head->Next = NULL;
        return 0;
    }

    Element* elementPtr = list->Head;
    while (elementPtr->Next != NULL)
    {
        elementPtr = elementPtr->Next;
    }

    elementPtr->Next = malloc(sizeof(Element));
    elementPtr->Next->Data = malloc(list->ObjectSize);
    elementPtr->Next->Data = data;
    elementPtr->Next->Next = NULL;

    return 0;
}

void FreePointer(Element* element)
{
    free(element->Data);
    free(element);
}

int RemoveDataFromList(LinkedList* list, void* data)
{
    if (data == NULL || list == NULL)
    {
        return -1;
    }
    
    if (list->Head == NULL)
    {
        return -1;
    }

    Element* elementPtr = GetHead(list);

    // Removing the head is an edge case
    if(elementPtr->Data == data)
    {

        list->Head = elementPtr->Next; // Might want to extract this into a function for clarity.
        FreePointer(elementPtr);     
        return 1;     
    }

    while(elementPtr != NULL)
    {
        if(elementPtr->Data == data)
        {
            list->LastAccessed->Next = elementPtr->Next;
            FreePointer(elementPtr);
            return 1;
        }
        elementPtr = GetNext(list); // We can use list->LastAccessed if we need to connect the pointers
    }

    return 0;
}

// This produces some unnessesary frees, which is a big problem.
// Something worth 4 bytes is causing the error.
// It's probably got something to do with the head, considering the head is the only element in the list.

int ClearList(LinkedList* list) 
{
    if (list == NULL)
    {
        return -1;
    }

    Element* elementPtr = GetHead(list);

    if(elementPtr == NULL)
    {
        return 0;
    }
    
    while(elementPtr != NULL)
    {
        RemoveDataFromList(list, elementPtr->Data);
        elementPtr = GetNext(list);
    }

    return 0;
}

int DestructList(LinkedList** list)
{
    if(list == NULL)
    {
        return -1;
    }
    ClearList(*list);
    free(*list);
    *list = NULL;
    list = NULL;
    return 0;
}

void* GetHead(LinkedList* list)
{
    if(list == NULL)
    {
        return NULL;
    }
    list->LastAccessed = list->Head;
    return list->Head;
}

void* GetNext(LinkedList* list)
{
    if(list->LastAccessed == NULL)
    {
        return NULL;
    }
    list->LastAccessed = list->LastAccessed->Next;
    return list->LastAccessed;
}
