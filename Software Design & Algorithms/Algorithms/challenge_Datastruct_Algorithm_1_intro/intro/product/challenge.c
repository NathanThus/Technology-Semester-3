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

    int min_index = 0;
    // Selection sort
    for (size_t i = 0; i < size - 1; i++)
    {
        for (size_t j = i + 1; j < size; j++)
        {
            if(array[j] < array[min_index])
            {
                min_index = j;
            }
        }
        
        if (min_index != i)
        {
            // Swap values
            int temp = array[i];
            array[i] = array[min_index];
            array[min_index] = temp;
        }
    }

    int smallest = 0, largest = 0;

    for (size_t i = 0; i < k; i++)
    {
        smallest += array[i];
    }
    for (size_t i = size - k; i < size; i++)
    {
        largest += array[i];
    }
    *difference = largest - smallest;

    return 1;
    
}

int ComputeDifferenceBetweenMaxAndMinSumOfKElements_1(
            int* array, int size, int k, int* difference)
{
    if(array == NULL || difference == NULL)
    {
        return -1;
    }
    // This is Bubble Sort

    for (size_t i = 0; i < size - 1; i++)
    {
        for (size_t j = 0; j < size - i - 1; j++)
        {
            if(array[j] > array[j + 1])
            {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
    

    int smallest = 0, largest = 0;

    for (size_t i = 0; i < k; i++)
    {
        smallest += array[i];
    }
    for (size_t i = size - k; i < size; i++)
    {
        largest += array[i];
    }
    *difference = largest - smallest;

    return 1;
}

int ComputeDifferenceBetweenMaxAndMinSumOfKElements_2(
            int* array, int size, int k, int* difference)
{
    if(array == NULL || difference == NULL)
    {
        return -1;
    }

    int key;
    // This is Insertion Sort
    for (size_t i = 1; i < size; i++)
    {
        key = array[i];
        int j = i - 1;

        while (j >= 0 && array[j] > key)
        {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = key;
    }

    int smallest = 0, largest = 0;

    for (size_t i = 0; i < k; i++)
    {
        smallest += array[i];
    }
    for (size_t i = size - k; i < size; i++)
    {
        largest += array[i];
    }
    *difference = largest - smallest;

    return 1;
}

int ComputeDifferenceBetweenMaxAndMinSumOfKElements_3(
            int* array, int size, int k, int* difference)
{
    if(array == NULL || difference == NULL)
    {
        return -1;
    }

    // Gnome Sort
    int index = 0;

    while (index < size)
    {
        if (index == 0)
        {
            index++;
        }
        if (array[index] >= array[index - 1])
        {
            index++;
        }
        else
        {
            int temp = array[index];
            array[index] = array[index - 1];
            array[index - 1] = temp;
            index--;
        }
    }

    int smallest = 0, largest = 0;

    for (size_t i = 0; i < k; i++)
    {
        smallest += array[i];
    }
    for (size_t i = size - k; i < size; i++)
    {
        largest += array[i];
    }
    *difference = largest - smallest;

    return 1;
}
