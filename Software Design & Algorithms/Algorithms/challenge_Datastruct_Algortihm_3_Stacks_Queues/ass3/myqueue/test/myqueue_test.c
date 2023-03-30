#include <string.h>
#include <stdio.h>
#include "mystack.h"
#include "myqueue.h"
#include "unity.h"
#include "testrunner.h"

// I rather dislike keeping line numbers updated, so I made my own macro to ditch the line number
#define MY_RUN_TEST(func) RUN_TEST(func, 0)

void setUp(void)
{
}

void tearDown(void)
{
    // This is run after EACH test
}

void test_queue_create_delete(void)
{
	QueueMeta_t* queue = myqueue_create(1);
	TEST_ASSERT_NOT_NULL(queue);
	myqueue_delete(queue);
}

void test_queue_create_invalidItemSize(void)
{
	TEST_ASSERT_NULL(myqueue_create(0));
	TEST_ASSERT_NULL(myqueue_create(-1));
}

void test_enqueue_dequeue(void)
{
	QueueMeta_t* queue = myqueue_create(1);
	TEST_ASSERT_NOT_NULL(queue);
	u_int8_t data = 0x55;
	TEST_ASSERT_EQUAL_INT(0, myqueue_enqueue(queue, &data));
	TEST_ASSERT_EQUAL_INT(0, myqueue_dequeue(queue, &data));
	TEST_ASSERT_EQUAL_INT(0x55, data);
	myqueue_delete(queue);
}

void test_enqueue_dequeue_Multiple(void)
{
	QueueMeta_t* queue = myqueue_create(1);
	TEST_ASSERT_NOT_NULL(queue);
	u_int8_t data = 0x55;
	TEST_ASSERT_EQUAL_INT(0, myqueue_enqueue(queue, &data));
	TEST_ASSERT_EQUAL_INT(0, myqueue_enqueue(queue, &data));
	TEST_ASSERT_EQUAL_INT(0, myqueue_enqueue(queue, &data));
	TEST_ASSERT_EQUAL_INT(0, myqueue_enqueue(queue, &data));

	TEST_ASSERT_EQUAL_INT(0, myqueue_dequeue(queue, &data));

	TEST_ASSERT_EQUAL_INT(0, mystack_nofelem(queue->stack_in));
	TEST_ASSERT_EQUAL_INT(4, mystack_nofelem(queue->stack_out));
}

void test_enqueue_dequeue_NullPtr(void)
{
	QueueMeta_t* queue = myqueue_create(1);
	TEST_ASSERT_NOT_NULL(queue);
	u_int8_t data = 0x55;
	TEST_ASSERT_EQUAL_INT(-1, myqueue_enqueue(queue, NULL));
	TEST_ASSERT_EQUAL_INT(-1, myqueue_dequeue(queue, NULL));

	TEST_ASSERT_EQUAL_INT(-1, myqueue_enqueue(NULL, &data));
	TEST_ASSERT_EQUAL_INT(-1, myqueue_dequeue(NULL, &data));
	myqueue_delete(queue);
}


int main () {
	UnityBegin();
	MY_RUN_TEST(test_queue_create_delete);
	MY_RUN_TEST(test_queue_create_invalidItemSize);
	MY_RUN_TEST(test_enqueue_dequeue);
	MY_RUN_TEST(test_enqueue_dequeue_Multiple);
	MY_RUN_TEST(test_enqueue_dequeue_NullPtr);
	return UnityEnd();
}
