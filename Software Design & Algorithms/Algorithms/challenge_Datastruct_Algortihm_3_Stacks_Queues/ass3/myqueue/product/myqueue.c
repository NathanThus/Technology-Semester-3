#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include "mystack.h"
#include "myqueue.h"

QueueMeta_t *myqueue_create(int item_size)
{
	if(item_size <= 0)
	{
		return NULL;
	}

	QueueMeta_t* queue = (QueueMeta_t*)malloc(sizeof(QueueMeta_t));
	queue->item_size = item_size;
	queue->stack_in = mystack_create(item_size);
	queue->stack_out = mystack_create(item_size);

	return queue;
}

void myqueue_delete(QueueMeta_t *queue)
{
	if(queue == NULL)
	{
		return;
	}

	mystack_destroy(queue->stack_in);
	mystack_destroy(queue->stack_out);
	free(queue);
}

int myqueue_enqueue(QueueMeta_t *que, void *obj)
{
	if(que == NULL || obj == NULL)
	{
		return -1;
	}

	mystack_push(que->stack_in, obj);
	return 0;
}

int myqueue_dequeue(QueueMeta_t *queue, void *obj)
{
	if(queue == NULL || obj == NULL)
	{
		return -1;
	}

	if(mystack_nofelem(queue->stack_out) == 0)
	{
		while(mystack_nofelem(queue->stack_in) > 0)
		{
			mystack_pop(queue->stack_in, obj);
			mystack_push(queue->stack_out, obj);
		}
	}

	return 0;
}




