#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "challenge.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Please provide argument to select assignment.\n");
        return -1;
    }
    char selectedAssignment = atoi(argv[1]);

    // TODO: parse data from stdin as explained in part 0 in the challenge document
    // (obviously: parsing data is another responsibility than commandline UI stuff,
    //  so parsing data should happen in another file!)

    if (selectedAssignment == 1)
    {
        int smallestNumber = 0;
        FindSmallestNumberThatIsRepeatedKTimes( /* TODO: fill in parameters */ );

        printf("%d\n", smallestNumber);
    }
    else if (selectedAssignment == 2)
    {
        int difference = 0;
        ComputeDifferenceBetweenMaxAndMinSumOfKElements_0( /* TODO: fill in parameters */ );

        printf("%d\n", difference);
    }
    else if (selectedAssignment == 31)
    {
        int difference = 0;
        ComputeDifferenceBetweenMaxAndMinSumOfKElements_1( /* TODO: fill in parameters */ );
        printf("%d\n", difference);
    }
    else if (selectedAssignment == 32)
    {
        int difference = 0;
        ComputeDifferenceBetweenMaxAndMinSumOfKElements_2( /* TODO: fill in parameters */ );
        printf("%d\n", difference);
    }
    else if (selectedAssignment == 33)
    {
        int difference = 0;
        ComputeDifferenceBetweenMaxAndMinSumOfKElements_3( /* TODO: fill in parameters */ );
        printf("%d\n", difference);
    }
    else
    {
        printf("Error: Unknown assignment: %d\n", selectedAssignment);
    }

    return 0;
}
