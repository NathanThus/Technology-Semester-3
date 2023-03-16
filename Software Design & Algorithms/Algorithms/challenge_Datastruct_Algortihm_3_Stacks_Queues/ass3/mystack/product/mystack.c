
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

StackMeta_t *mystack_create(size_t objsize)
{
 	return NULL;
}

int mystack_push(StackMeta_t *stack, void* obj)
{
	return 0;
}

int mystack_pop(StackMeta_t *stack, void* obj)
{
      	return 0;
}

void mystack_destroy(StackMeta_t *stack)
{
	return;
}

int mystack_nofelem(StackMeta_t *stack)
{
	return 0;
}
