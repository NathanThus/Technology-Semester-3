#include "parse.h"
#include <stddef.h>
#include <stdio.h>

#define FileArgument 2

int GetNumberOfValues(int* n)
{
    if (n == NULL)
    {
        return -1;
    }
    return scanf("%d",n);
}

int GetArray(int* array, int size)
{
    if (array == NULL)
    {
        return -1;
    }

    for (size_t i = 0; i < size; i++)
    {
        if(scanf("%d", &array[i]) == 0)
        {
            return 0;
        }
    }
    
    return 1;
}
int GetSearchParameter(int* k)
{
    if(k == NULL)
    {
        return -1;
    }

    return scanf("%d",k);
}