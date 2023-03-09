#include "linked_list.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void AllocatePointer(Element* element, void* data, int objectSize)
{
    if(element == NULL || data == NULL || objectSize < 1)
    {
        return;
    }

    element->Data = malloc(objectSize);
    memcpy(element->Data, data, objectSize);
    element->Next = NULL;
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
        AllocatePointer(list->Head, data, list->ObjectSize);
        return 0;
    }

    Element* elementPtr = list->Head;
    while (elementPtr->Next != NULL)
    {
        elementPtr = elementPtr->Next;
    }

    elementPtr->Next = malloc(sizeof(Element));
    AllocatePointer(elementPtr->Next, data, list->ObjectSize);

    list->LastAccessed = elementPtr;

    return 0;
}

void FreePointer(Element* element)
{
    if(element == NULL)
    {
        return;
    }

    if(element->Data != NULL)
    {
        free(element->Data);
    }

    free(element);
}

int RemoveDataFromList(LinkedList* list, int index)
{
    if (list == NULL || index < 0)
    {
        return -1;
    }
    
    if (list->Head == NULL)
    {
        return 0;
    }

    Element* elementPtr = GetHead(list);

    if (index == 0)
    {
        list->Head = elementPtr->Next;
        FreePointer(elementPtr);
        return 1;
    }

    int i = 0;
    while (i < index - 1)
    {
        elementPtr = GetNext(list);
        i++;
    }

    Element* elementToRemove = GetNext(list);
    elementPtr->Next = elementToRemove->Next;
    FreePointer(elementToRemove);

    return 1;
}

int ClearList(LinkedList* list) 
{
    if (list == NULL)
    {
        return -1;
    }

    while (GetHead(list) != NULL)
    {
        RemoveDataFromList(list,0);
    }

    list->LastAccessed = NULL;

    return 0;
}

int DestructList(LinkedList** list)
{
    if(*list == NULL)
    {
        return -1;
    }

    ClearList(*list);
    free(*list);

    *list = NULL;
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

void* RetrieveData(LinkedList *list, void *data)
{
    if (data == NULL || list == NULL)
    {
        return NULL;
    }
    return ((Element*)data)->Data;
}
