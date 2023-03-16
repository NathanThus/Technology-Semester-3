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

void run_my_test()
{
	printf("Please specify your tests\n");
}
int main (int argc, char * argv[]) {
	UnityBegin();
	MY_RUN_TEST(run_my_test);
	return UnityEnd();
}
