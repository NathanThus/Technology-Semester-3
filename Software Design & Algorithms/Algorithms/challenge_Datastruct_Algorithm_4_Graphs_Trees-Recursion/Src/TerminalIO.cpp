#include "TerminalIO.hpp"

#include <iostream>

// NAMESPACE
using std::cin;
using std::cout;
using std::endl;

// TEST CASES
#define MINIMUM_NUMBER_OF_TEST_CASES 1
#define MAXIMUM_NUMBER_OF_TEST_CASES 10

// NODES
#define MINIMUM_NUMBER_OF_NODES 1
#define MAXIMUM_NUMBER_OF_NODES 1000000

// CONNECTIONS
#define MINIMUM_NUMBER_OF_CONNECTIONS 1
#define MAXIMUM_NUMBER_OF_CONNECTIONS 10000


int TerminalIO::GetNumberOfTestCases(int* numberOfTestCases)
{
    if( numberOfTestCases == nullptr)
    {
        return -1;
    }

    cin >> *numberOfTestCases;

    if(*numberOfTestCases < MINIMUM_NUMBER_OF_TEST_CASES || *numberOfTestCases > MAXIMUM_NUMBER_OF_TEST_CASES)
    {
        return -1;
    }

    return 1;
}

int TerminalIO::GetTestParameters(int* goal, int* connections)
{
    if(goal == nullptr || connections == nullptr)
    {
        return -1;
    }

    cin >> *goal;
    cin >> *connections;

    if(*goal < MINIMUM_NUMBER_OF_NODES || *goal > MAXIMUM_NUMBER_OF_NODES)
    {
        return -1;
    }

    if(*connections < MINIMUM_NUMBER_OF_CONNECTIONS || *connections > MAXIMUM_NUMBER_OF_CONNECTIONS)
    {
        return -1;
    }

    return 1;
}

int TerminalIO::GetConnection(int* source, int* destination, int goal)
{
    if(source == nullptr || destination == nullptr)
    {
        return -1;
    }

    cin >> *source;
    cin >> *destination;

    if(*source > goal || *source < MINIMUM_NUMBER_OF_CONNECTIONS)
    {
        return -1;
    }

    if(*destination > goal || *destination < MINIMUM_NUMBER_OF_CONNECTIONS)
    {
        return -1;
    }

    return 0;
}

void TerminalIO::PrintResult(int result)
{
    cout << result << endl;
}

void TerminalIO::PrintText(std::string error)
{
    cout << error << endl;
}

MENU_CHOICE TerminalIO::GetMenuChoice(const char *menuChoice)
{
    if(menuChoice == nullptr)
    {
        return MENU_CHOICE::INVALID;
    }

    if(strcmp(menuChoice, "TREE") == 0)
    {
        return MENU_CHOICE::TREE;
    }
    if(strcmp(menuChoice, "RECURSION") == 0)
    {
        return MENU_CHOICE::RECURSION;
    }

    return MENU_CHOICE::INVALID;
}
