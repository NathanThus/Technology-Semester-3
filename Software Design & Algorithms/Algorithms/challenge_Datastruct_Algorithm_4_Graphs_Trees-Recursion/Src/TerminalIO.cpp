#include "TerminalIO.hpp"

#include <iostream>
#include <cmath>

// NAMESPACE
using std::cin;
using std::cout;
using std::endl;

// TEST CASES
#define MINIMUM_NUMBER_OF_TEST_CASES 1
#define MAXIMUM_NUMBER_OF_TEST_CASES 10

// NODES
#define MINIMUM_NUMBER_OF_NODES 1
#define MAXIMUM_NUMBER_OF_NODES pow(10, 4)

// CONNECTIONS
#define MINIMUM_NUMBER_OF_CONNECTIONS 1
#define MAXIMUM_NUMBER_OF_CONNECTIONS pow(10, 5)


int TerminalIO::GetNumberOfTestCases(int* numberOfTestCases)
{
    if( numberOfTestCases == nullptr)
    {
        return -1;
    }

    cin >> *numberOfTestCases;
    cout << "T: " << *numberOfTestCases << endl;

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
    cout << "N (goal / max): " << *goal << endl;
    cin >> *connections;
    cout << "M (# of nodes): " << *connections << endl;

    if(*goal < MINIMUM_NUMBER_OF_NODES || *goal > MAXIMUM_NUMBER_OF_NODES)
    {
        PrintText("Invalid number of nodes");
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
        cout << "Source: " << *source << endl;
        PrintText("Invalid source");
        return -1;
    }

    if(*destination > goal || *destination < MINIMUM_NUMBER_OF_CONNECTIONS)
    {
        PrintText("Invalid destination");
        return -1;
    }

    if(*source == *destination)
    {
        PrintText("Source and destination cannot be the same");
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
