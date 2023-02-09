#include <string.h>

#include "challenge.h"
#include "unity.h"

// I rather dislike keeping line numbers updated, so I made my own macro to ditch the line number
#define MY_RUN_TEST(func) RUN_TEST(func, 0)


void setUp(void)
{
    // This is run before EACH test
}

void tearDown(void)
{
  // This is run after EACH test
}

void test_find_smallest_number_for_number_of_wanted_occurences(void)
{
  int array[] = {10,10,2,2,1,1,1,4,5,4};
  int arraySize = sizeof(array)/sizeof(array[0]);
  int K = 2;
  int smallestNumber = 0;

  TEST_ASSERT_EQUAL(0, FindSmallestNumberThatIsRepeatedKTimes(
    array, arraySize, K, &smallestNumber) );

  TEST_ASSERT_EQUAL(2, smallestNumber);
}

void test_ComputeDifferenceBetweenMaxAndMinSumOfKElements(void)
{
  int array[] = {10,10,2,2,1,1,1,4,5,4};
  int arraySize = sizeof(array)/sizeof(array[0]);
  int K = 2;
  int difference = 0;

  TEST_ASSERT_EQUAL(0, ComputeDifferenceBetweenMaxAndMinSumOfKElements_0(
  array, arraySize, K, &difference) );

  TEST_ASSERT_EQUAL(18, difference);
}


void test_ComputeDifferenceBetweenMaxAndMinSumOfKElements_DifferentLargest(void)
{
  int array[] = {11,10,2,2,1,1,1,4,5,4};
  int arraySize = sizeof(array)/sizeof(array[0]);
  int K = 2;
  int difference = 0;

  TEST_ASSERT_EQUAL(0, ComputeDifferenceBetweenMaxAndMinSumOfKElements_0(
  array, arraySize, K, &difference) );

  TEST_ASSERT_EQUAL(19, difference);
}

// Effectively a stress test
void test_ComputeDifferenceBetweenMaxAndMinSumOfKElements_Massive(void)
{
  int array[] = {816,291,321,704,100,257,170,665,202,598,304,607,196,136,28,707,462,931,679,573,451,962,982,948,218,317,286,20,166,822,669,981,113,341,36,212,597,558,877,151,507,180,757,702,316,136,408,129,67,439,701,517,752,683,816,969,351,453,340,868,275,9,848,387,349,236,950,298,793,826,448,299,358,556,352,673,691,111,153,757,901,854,625,653,888};
  int arraySize = 85;
  int K = 26;
  int difference = 0;

  TEST_ASSERT_EQUAL(0, ComputeDifferenceBetweenMaxAndMinSumOfKElements_0(
  array, arraySize, K, &difference) );

  TEST_ASSERT_EQUAL(17627, difference);
}


int main (int argc, char * argv[])
{
    UnityBegin();

    MY_RUN_TEST(test_find_smallest_number_for_number_of_wanted_occurences);
    MY_RUN_TEST(test_ComputeDifferenceBetweenMaxAndMinSumOfKElements);
    MY_RUN_TEST(test_ComputeDifferenceBetweenMaxAndMinSumOfKElements_DifferentLargest);
    MY_RUN_TEST(test_ComputeDifferenceBetweenMaxAndMinSumOfKElements_Massive);
    return UnityEnd();
}
