#include "linked_list.h"

#include <stdlib.h>

static Element* head = NULL;

/* function: ListAddTail
 * pre: -
 * post: an element is added to the end of the linked list
 * returns: 0 on success, -1 on fail
 */
int ListAddTail(int address, int size)
{
     if (address < 0 || size < 0)
    {
        return -1;
    }

    if(head == NULL)
    {
        head = (Element*)malloc(sizeof(Element));
        head->address = address;
        head->size = size;
        head->next = NULL;
        return 0;
    }

    Element* temp = head;
    while(temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = (Element*)malloc(sizeof(Element));
    temp->next->address = address;
    temp->next->size = size;
    temp->next->next = NULL;

    return 0;
}

/* function: ListAddAfter
 * pre: -
 * post: a new element with given data is added after element
 *       in the linked list. If element is NULL, the new element
 *       is added to the front of the list.
 * returns: 0 on success, -1 on fail
 */
int ListAddAfter(int address, int size, Element* element)
{
    if (address < 0 || size < 0)
    {
        return -1;
    }

    if(element == NULL)
    {
        Element* newElement = (Element*)malloc(sizeof(Element));
        newElement->address = address;
        newElement->size = size;

        Element* tempHead = head;
        head = newElement;
        newElement->next = tempHead;
    }
    else
    {
        Element* newElement = (Element*)malloc(sizeof(Element));
        newElement->address = address;
        newElement->size = size;

        newElement->next = element->next;
        element->next = newElement;
    }

    return 0;

}


/* function: ListGetHead
 * pre: -
 * post: first element in linked list is returned
 * returns: first element on success, NULL if list is empty
 */
Element* ListGetHead()
{
    return head;
}


/* function: ListRemoveTail
 * pre: -
 * post: last element is removed from list
 * returns: 0 on success, -1 on fail
 */
int ListRemoveTail()
{
    if(head == NULL)
    {
        return -1;
    }
    if(head->next == NULL)
    {
        free(head);
        head = NULL;
        return 0;
    }
    Element* temp = head;
    while(temp->next->next != NULL)
    {
        temp = temp->next;
    }
    free(temp->next);
    temp->next = NULL;
    temp = NULL;
    return 0;
}

/* function: ListRemove
 * pre: -
 * post: element is removed from list, pointer from callee is made NULL
 * returns: 0 on success, -1 on fail
 */
int ListRemove(Element** element)
{
    if(*element == NULL)
    {
        return -1;
    }
    if(*element == head)
    {
        head = head->next;
        free(*element);
        *element = NULL;
        return 0;
    }
    Element* temp = head;
    while(temp->next != *element)
    {
        temp = temp->next;
    }
    temp->next = (*element)->next;
    free(*element);
    *element = NULL;
    return -1;
}

/* function: ListRemoveAll
 * pre: -
 * post: all existing elements from list are removed
 */
void ListRemoveAll()
{
    while(head != NULL)
    {
        ListRemoveTail();
    }
}
