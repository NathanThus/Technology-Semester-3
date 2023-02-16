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
        // list->Head->Data = malloc(list->ObjectSize);
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
    // elementPtr->Next->Data = malloc(list->ObjectSize);
    elementPtr->Next->Data = data;
    elementPtr->Next->Next = NULL;

    return 0;
}


int RemoveDataFromList(LinkedList* list, void* data)
{
    if (data == NULL || list == NULL)
    {
        return -1;
    }
    Element* prevPtr = NULL;
    Element* currentPtr = list->Head;

    while (currentPtr != NULL)
    {
        prevPtr = currentPtr;
        if (currentPtr->Data == data)
        {
            Element* nextPtr = currentPtr->Next;
            // free(currentPtr->Data);
            prevPtr->Next = nextPtr;
            free(currentPtr);
            return 1;
        }
        currentPtr = currentPtr->Next;
    }
    return 0;
}

int ClearList(LinkedList* list)
{
    if (list == NULL)
    {
        return -1;
    }

    Element* elementPtr = list->Head;
    Element* removerPtr;
    while (elementPtr->Next != NULL)
    {
        removerPtr = elementPtr;
        elementPtr = elementPtr->Next; // INVALID READ
        // free(removerPtr->Data);
        free(removerPtr);
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
    return list->Head->Data;
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
