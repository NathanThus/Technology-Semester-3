#include <string.h>
#include <stdio.h>

#include "mystack.h"
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

void test_myStack_Create_Valid()
{
	StackMeta_t *stack = mystack_create(1);
	TEST_ASSERT_NOT_NULL(stack);
	TEST_ASSERT_EQUAL_INT(0, mystack_nofelem(stack));
	TEST_ASSERT_EQUAL_INT(1, stack->objsize);
	mystack_destroy(stack);
}

void test_myStack_Create_InvalidObjectSize_Zero()
{
	StackMeta_t *stack = mystack_create(0);
	TEST_ASSERT_NULL(stack);
}

void test_myStack_push_InvalidStack()
{
	StackMeta_t *stack = NULL;
	int result = mystack_push(stack, NULL);
	TEST_ASSERT_EQUAL_INT(-1, result);
}

void test_myStack_push_InvalidObject()
{
	StackMeta_t *stack = mystack_create(1);
	int result = mystack_push(stack, NULL);
	TEST_ASSERT_EQUAL_INT(-1, result);
	mystack_destroy(stack);
}

void test_myStack_push_Valid()
{
	StackMeta_t *stack = mystack_create(sizeof(int));
	int data = 1;
	int result = mystack_push(stack, &data);
	TEST_ASSERT_EQUAL_INT(0, result);
	TEST_ASSERT_EQUAL_INT(1, mystack_nofelem(stack));
	TEST_ASSERT_NOT_NULL(stack->stack);
	mystack_destroy(stack);
}

void test_myStack_pop_InvalidStack()
{
	StackMeta_t *stack = NULL;
	int result = mystack_pop(stack, NULL);
	TEST_ASSERT_EQUAL_INT(-1, result);
}

void test_myStack_pop_NoElements()
{
	StackMeta_t *stack = mystack_create(1);
	int result = mystack_pop(stack, NULL);
	TEST_ASSERT_EQUAL_INT(-1, result);
	mystack_destroy(stack);
}

void test_myStack_pop_Valid()
{
	StackMeta_t *stack = mystack_create(sizeof(int));
	int data = 1;
	int result = mystack_push(stack, &data);
	TEST_ASSERT_EQUAL_INT(0, result);
	TEST_ASSERT_EQUAL_INT(1, mystack_nofelem(stack)); // There is data!
	data = 0;
	result = mystack_pop(stack, &data);
	TEST_ASSERT_EQUAL_INT(0, result);
	TEST_ASSERT_EQUAL_INT(1, data); // The data was popped!
	TEST_ASSERT_EQUAL_INT(0, mystack_nofelem(stack)); // There is no data!
	mystack_destroy(stack);
}

void test_mystack_nofelem_InvalidStack()
{
	StackMeta_t *stack = NULL;
	int result = mystack_nofelem(stack);
	TEST_ASSERT_EQUAL_INT(-1, result);
}

void test_mystack_nofelem_Valid()
{
	StackMeta_t *stack = mystack_create(1);
	int result = mystack_push(stack, "a");
	TEST_ASSERT_EQUAL_INT(0, result);
	TEST_ASSERT_EQUAL_INT(1, mystack_nofelem(stack));
	mystack_destroy(stack);
}

void test_mystack_nofelem_EmptyStack()
{
	StackMeta_t *stack = mystack_create(1);
	TEST_ASSERT_EQUAL_INT(0, mystack_nofelem(stack));
	mystack_destroy(stack);
}

int main (int argc, char * argv[]) {
	UnityBegin();
	MY_RUN_TEST(test_myStack_Create_Valid);
	MY_RUN_TEST(test_myStack_Create_InvalidObjectSize_Zero);
	MY_RUN_TEST(test_myStack_push_InvalidStack);
	MY_RUN_TEST(test_myStack_push_InvalidObject);
	MY_RUN_TEST(test_myStack_push_Valid);
	MY_RUN_TEST(test_myStack_pop_InvalidStack);
	MY_RUN_TEST(test_myStack_pop_NoElements);
	MY_RUN_TEST(test_myStack_pop_Valid);
	MY_RUN_TEST(test_mystack_nofelem_InvalidStack);
	MY_RUN_TEST(test_mystack_nofelem_Valid);
	MY_RUN_TEST(test_mystack_nofelem_EmptyStack);
	return UnityEnd();
}
