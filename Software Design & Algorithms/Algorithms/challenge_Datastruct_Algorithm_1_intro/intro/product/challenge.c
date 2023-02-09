#include "challenge.h"
#include <stddef.h>
#include <stdbool.h>


//Remove
#include <stdio.h>

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
            *smallestNumber = i;
            return 0;
        }
    }
    return 1;
}

int ComputeDifferenceBetweenMaxAndMinSumOfKElements_0(
            int* array, int size, int k, int* difference)
{
    if(array == NULL || difference == NULL)
    {
        return -1;
    }
    
    k *= 2;

    int ComputeArray[k];

    for (size_t i = 0; i < k / 2; i++)
    {
        ComputeArray[i] = 100000;
    }

    for (size_t i = k / 2; i < k; i++)
    {
        ComputeArray[i] = 0;
    }
    bool wasLower = false;

    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < k / 2; j++)
        {
            if(array[i] < ComputeArray[j])
            {
                ComputeArray[j] = array[i];
                wasLower = true;
            }
        }


        for (size_t j = k / 2; (j < k) && !wasLower; j++)
        {
            if(array[i] > ComputeArray[j])
            {
                ComputeArray[j] = array[i];
            }
        }
        wasLower = false;
    }

    int min = 0, max = 0;

    for (size_t i = 0; i < k; i++)
    {
        if(i < k/2)
        {
            min += ComputeArray[i];
        }
        else if(i >= k/2)
        {
            max += ComputeArray[i];
        }
    }

    *difference = max - min;

    return 0;
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
