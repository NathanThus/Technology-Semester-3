// System Includes
// My Includes
#include "TerminalIO.hpp"
#include "Tree.hpp"

// My Defines
#define REQUESTED_TEST 1
#define TREE "TREE"
#define RECURSION "RECURSION"

TerminalIO terminalIO;

int HandleTree()
{
    int testCases = 0;
    if(terminalIO.GetNumberOfTestCases(&testCases) == -1)
    {
        terminalIO.PrintText("Invalid number of test cases");
        return -1;
    }
    int goal = 0;
    int connections = 0;
    if(terminalIO.GetTestParameters(&goal, &connections) == -1)
    {
        terminalIO.PrintText("Invalid number of nodes or connections");
        return -1;
    }

    int source = 0;
    int destination = 0;
    if(terminalIO.GetConnection(&source, &destination, goal) == -1)
    {
        terminalIO.PrintText("Invalid connection");
        return -1;
    }

    Tree tree;
    tree.GenerateChildren(goal);

    for (int i = 0; i < connections; i++)
    {
        int sourceLeafID = 0, destinationLeafID = 0;
        terminalIO.GetConnection(&sourceLeafID, &destinationLeafID, goal);
        tree.AddConnection(sourceLeafID, destinationLeafID);
    }
    

    return 0;
}

int HandleRecursion()
{
    terminalIO.PrintText("Not implemented");
    return -1; // NOT IMPLEMENTED
}

int main(int argc, char const *argv[])
{
    if(argc < REQUESTED_TEST)
    {
        terminalIO.PrintText("Not enough arguments");
        return -1;
    }

    if(strcmp(argv[REQUESTED_TEST], TREE) == 0)
    {
        return HandleTree();
    }
    if(strcmp(argv[REQUESTED_TEST], RECURSION) == 0)
    {
        return HandleRecursion();
    }
    return 0;
}