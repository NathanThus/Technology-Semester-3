#include "TerminalIO.hpp"

// TEST CASES
#define MINIMUM_NUMBER_OF_TEST_CASES 1
#define MAXIMUM_NUMBER_OF_TEST_CASES 10

// NODES
#define MINIMUM_NUMBER_OF_NODES 1
#define MAXIMUM_NUMBER_OF_NODES 10^5

// CONNECTIONS
#define MINIMUM_NUMBER_OF_CONNECTIONS 1
#define MAXIMUM_NUMBER_OF_CONNECTIONS 10^4

int TerminalIO::GetNumberOfTestCases(FILE* stream)
{
    if(stream == nullptr)
    {
        return -1;
    }

    int numberOfTestCases = 0;
    fscanf(stream,"%d", &numberOfTestCases);
    if(numberOfTestCases < MINIMUM_NUMBER_OF_TEST_CASES && numberOfTestCases > MAXIMUM_NUMBER_OF_TEST_CASES)
    {
       return -1; 
    }
    return numberOfTestCases;
}

int TerminalIO::GetNumberOfNodes(FILE* stream)
{
    if(stream == nullptr)
    {
        return -1;
    }

    int numberOfNodes = 0;
    fscanf(stream,"%d", &numberOfNodes);

    if(numberOfNodes < MINIMUM_NUMBER_OF_NODES && numberOfNodes > MAXIMUM_NUMBER_OF_NODES)
    {
        return -1;
    }

    return numberOfNodes;
}

int TerminalIO::GetNumberOfConnections(FILE* stream)
{
    if(stream == nullptr)
    {
        return -1;
    }

    int numberOfConnections = 0;
    fscanf(stream,"%d", &numberOfConnections);
    return numberOfConnections;
}

void TerminalIO::PrintResult(FILE* stream, int *result, int numberOfTestCases)
{
    if (result == nullptr)
    {
        return;
    }

    for (size_t i = 0; i < numberOfTestCases; i++)
    {
        fprintf(stream,"%d\n", result[i]);
    }
}
