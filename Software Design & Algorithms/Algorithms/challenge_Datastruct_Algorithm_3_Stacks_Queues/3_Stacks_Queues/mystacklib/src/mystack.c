#include "mystack.h"
#include "logging.h"
/* The stack is an abstract data type.
 * this means that the internal structures are
 * never exposed to the users of the library.
 */

/* Note: the stacks have no knowledge of what types
 * they are storing. This is not a concern of the stack
 */

typedef struct stackObject
{
	void* obj;
	struct stackObject* next;
} StackObject_t;

typedef struct stackMeta
{
	StackObject_t* stack;
	size_t objsize;
	int numelem;
	int handle;
	struct stackMeta* next;
} StackMeta_t;

StackMeta_t* gStackList = NULL;


int MystackCreate(size_t objsize)
{
    /* Hint: use gStackList */
    int handle = 1;
    DBG_PRINTF("Created stack with handle: %d and objsize %zu bytes\n", handle, objsize);
    return 0;
}

int MystackPush(int handle, void* obj)
{
    DBG_PRINTF("handle: %d\n, obj: %p\n", handle, obj); 	
    return 0;
}

int MystackPop(int handle, void* obj)
{
    DBG_PRINTF("handle: %d\n, obj: %p\n", handle, obj); 	
    return 0;
}

int MystackDestroy(int handle)
{
    DBG_PRINTF("handle: %d\n", handle); 	
    return 0;
}

int MystackNofElem(int handle)
{
    DBG_PRINTF("handle: %d\n", handle);
    return 0;
}
