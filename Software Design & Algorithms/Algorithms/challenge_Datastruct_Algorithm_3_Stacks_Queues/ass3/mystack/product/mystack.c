#include "mystack.h"
#include "logging.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
/* The stack is an abstract data type.
* this means that the internal structures are
* never exposed to the users of the library.
*/

/* Note: the stacks have no knowledge of what types
* they are storing. This is not a concern of the stack
*/

StackMeta_t* Stack = NULL;

StackMeta_t* mystack_create(size_t objsize)
{
	if(objsize < 1)
	{
		return NULL;
	}

	Stack = malloc(sizeof(StackMeta_t));

	if(Stack == NULL)
	{
		return NULL;
	}

	Stack->numelem = 0;
	Stack->objsize = objsize;
	Stack->stack = NULL;

 	return Stack;
}

int mystack_push(StackMeta_t *stack, void* obj)
{
	if(stack == NULL || obj == NULL)
	{
		return -1;
	}

	StackObject_t* newObject = malloc(sizeof(StackObject_t));
	if(newObject == NULL)
	{
		return -1;
	}

	newObject->obj = malloc(stack->objsize);

	if(newObject->obj == NULL)
	{
		return -1;
	}

	memcpy(newObject->obj, obj, stack->objsize);
	newObject->next = NULL;

	StackObject_t* formerHeadPtr = stack->stack;
	newObject->next = formerHeadPtr;
	stack->stack = newObject;

	stack->numelem++;
	
	return 0;
}

int mystack_pop(StackMeta_t *stack, void* obj)
{
	if(stack == NULL || obj == NULL)
	{
		return -1;
	}

	if(stack->stack == NULL)
	{
		return -1;
	}

	StackObject_t* headPtr = stack->stack;
	StackObject_t* nextPtr = headPtr->next;

	memcpy(obj, headPtr->obj, stack->objsize);
	stack->stack = nextPtr;
	free (headPtr->obj);
	free (headPtr);

	stack->numelem--;
	
	return 0;
}

void mystack_destroy(StackMeta_t *stack)
{
	if (stack == NULL)
    {
        return;
    }

	// Clear the Stack

	StackObject_t* headPtr = stack->stack;
	StackObject_t* nextPtr = NULL;

	while(headPtr != NULL)
	{
		nextPtr = headPtr->next;
		free(headPtr->obj);
		free(headPtr);
		headPtr = nextPtr;
	}

	free(stack);
}

int mystack_nofelem(StackMeta_t *stack)
{
	if(stack == NULL)
	{
		return -1;
	}
	else
	{
		return stack->numelem;
	}
}
