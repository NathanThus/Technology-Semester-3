#include "challenge.h"
#include <stddef.h>

#define MAXIMUM_N 100000

int FindSmallestNumberThatIsRepeatedKTimes(
            int* array, int size, int k, int* smallestNumber)
{
    if(array == NULL || smallestNumber == NULL)
    {
        return -1;
    }

    int findSmallestArray[MAXIMUM_N];

    for (size_t i = 0; i < size; i++)
    {
        findSmallestArray[array[i]]++;
    }

    for (size_t i = 0; i < size; i++)
    {
        if(findSmallestArray[i] == k)
        {
            *smallestNumber = findSmallestArray[i];
            return 0;
        }
    }
    return 1;
}

int ComputeDifferenceBetweenMaxAndMinSumOfKElements_0(
            int* array, int size, int k, int* difference)
{
    return -1;
}

int ComputeDifferenceBetweenMaxAndMinSumOfKElements_1(
            int* array, int size, int k, int* difference)
{
    return -1;
}

int ComputeDifferenceBetweenMaxAndMinSumOfKElements_2(
            int* array, int size, int k, int* difference)
{
    return -1;
}

int ComputeDifferenceBetweenMaxAndMinSumOfKElements_3(
            int* array, int size, int k, int* difference)
{
    return -1;
}
