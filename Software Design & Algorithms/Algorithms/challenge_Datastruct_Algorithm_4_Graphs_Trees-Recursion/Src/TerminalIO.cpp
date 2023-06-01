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
#define MAXIMUM_NUMBER_OF_CONNECTIONS_GRAPH pow(10, 5)
#define MAXIMUM_NUMBER_OF_CONNECTIONS_TREE 20 - 1


int TerminalIO::GetNumberOfTestCases(int& numberOfTestCases)
{
    cin >> numberOfTestCases;

    if(numberOfTestCases < MINIMUM_NUMBER_OF_TEST_CASES || numberOfTestCases > MAXIMUM_NUMBER_OF_TEST_CASES)
    {
        return -1;
    }

    return 1;
}

int TerminalIO::GetNumberOfElements(int &numberOfElements)
{
    cin >> numberOfElements;

    if(numberOfElements < MINIMUM_NUMBER_OF_CONNECTIONS || numberOfElements > MAXIMUM_NUMBER_OF_CONNECTIONS_TREE)
    {
        return -1;
    }

    return 1;
}

int TerminalIO::GetTestParameters_Graph(int& goal, int& connections)
{
    cin >> goal;
    cin >> connections;

    if(goal < MINIMUM_NUMBER_OF_NODES || goal > MAXIMUM_NUMBER_OF_NODES)
    {
        PrintText("Invalid number of nodes");
        return -1;
    }

    if(connections < MINIMUM_NUMBER_OF_CONNECTIONS || connections > MAXIMUM_NUMBER_OF_CONNECTIONS_GRAPH)
    {
        
        return -1;
    }

    return 1;
}

int TerminalIO::GetGraphConnection(int& source, int& destination, int goal)
{
    cin >> source;
    cin >> destination;

    if(source > goal || source < MINIMUM_NUMBER_OF_CONNECTIONS)
    {
        cout << "ERROR: Source: " << source << endl;
        PrintText("ERROR: Invalid source");
        return -1;
    }
    if(destination > goal || destination < MINIMUM_NUMBER_OF_CONNECTIONS)
    {
        PrintText("ERROR: Invalid destination");
        return -1;
    }

    if(source == destination)
    {
        PrintText("ERROR: Source and destination are the same");
        return -1;
    }

    return 0;
}

int TerminalIO::GetTreeConnection(int &source, int &left, int &right)
{
    cin >> source;
    cin >> left;
    cin >> right;

    if(source <= 0 || source > MAXIMUM_NUMBER_OF_CONNECTIONS_TREE)
    {
        PrintText("ERROR: Invalid source");
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
