#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <realtime.h>

#include "challenge.h"
#include "parse.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Please provide argument to select assignment.\n");
        return -1;
    }
    char selectedAssignment = atoi(argv[1]);

    int n = 0;
    if(GetNumberOfValues(&n) != 1)
    {
        printf("Something went wrong with parsing N!\n");
        return -1;
    }

    int data[n];

    if(GetArray(&data[0],n) != 1)
    {
        printf("Something went wrong with parsing the data into the array! \n");
        return -1;
    }
    

    int k = 0;
    if(GetSearchParameter(&k) != 1)
    {
        printf("Something went wrong with parsing the search parameter!\n");
        return -1;
    }
    // TODO: parse data from stdin as explained in part 0 in the challenge document
    // (obviously: parsing data is another responsibility than commandline UI stuff,
    //  so parsing data should happen in another file!)

    if (selectedAssignment == 1)
    {
        int smallestNumber = 0;
        FindSmallestNumberThatIsRepeatedKTimes(&data[0],n,k,&smallestNumber);

        printf("%d\n", smallestNumber);
    }
    else if (selectedAssignment == 2)
    {
        int difference = 0;
        ComputeDifferenceBetweenMaxAndMinSumOfKElements_0(&data[0],n,k,&difference);

        printf("%d\n", difference);
    }
    else if (selectedAssignment == 31)
    {
        double functionTime = 0, timeEnd = 0;
        int difference = 0;

        printf("Bubble Sort\n");
        functionTime = getRealTime();
        ComputeDifferenceBetweenMaxAndMinSumOfKElements_1(&data[0],n,k,&difference);
        timeEnd = getRealTime();
        printf("Difference: %d\nTime: %lf \n", difference, timeEnd - functionTime);
    }
    else if (selectedAssignment == 32)
    {
        double functionTime = 0, timeEnd = 0;
        int difference = 0;

        printf("Insertion Sort\n");
        functionTime = getRealTime();
        ComputeDifferenceBetweenMaxAndMinSumOfKElements_2(&data[0],n,k,&difference);
        timeEnd = getRealTime();
        printf("Difference: %d\nTime: %lf \n", difference, timeEnd - functionTime);
    }
    else if (selectedAssignment == 33)
    {
        double functionTime = 0, timeEnd = 0;
        int difference = 0;

        printf("Gnome Sort\n");
        functionTime = clock();
        ComputeDifferenceBetweenMaxAndMinSumOfKElements_3(&data[0],n,k,&difference);
        timeEnd = clock();
        printf("Difference: %d\nTime: %ld\n", difference, timeEnd - functionTime);
    }
    else if(selectedAssignment == 34)
    {
        time_t functionTime = 0, timeEnd = 0;
        int difference = 0;

        printf("Bubble Sort\n");
        functionTime = clock();
        ComputeDifferenceBetweenMaxAndMinSumOfKElements_1(&data[0],n,k,&difference);
        timeEnd = clock();
        printf("Difference: %d\nTime: %ld\n", difference, timeEnd - functionTime);

        printf("Insertion Sort\n");
        functionTime = clock();
        ComputeDifferenceBetweenMaxAndMinSumOfKElements_2(&data[0],n,k,&difference);
        timeEnd = clock();
        printf("Difference: %d\nTime: %ld\n", difference, timeEnd - functionTime);

        printf("Gnome Sort\n");
        functionTime = clock();
        ComputeDifferenceBetweenMaxAndMinSumOfKElements_3(&data[0],n,k,&difference);
        timeEnd = clock();
        printf("Difference: %d\nTime: %ld\n", difference, timeEnd - functionTime);
    }
    else
    {
        printf("Error: Unknown assignment: %d\n", selectedAssignment);
    }

    return 0;
}
