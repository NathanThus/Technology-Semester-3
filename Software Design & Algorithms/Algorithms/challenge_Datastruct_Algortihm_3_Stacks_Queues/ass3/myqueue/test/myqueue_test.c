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
}

void test_enqueue_dequeue(void)
{
}


int main () {
	UnityBegin();
	MY_RUN_TEST(test_queue_create_delete);
	MY_RUN_TEST(test_enqueue_dequeue);
	return UnityEnd();
}
