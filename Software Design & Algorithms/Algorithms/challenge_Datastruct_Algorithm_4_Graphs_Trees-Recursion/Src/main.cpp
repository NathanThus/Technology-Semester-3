// System Includes
// My Includes
#include "TerminalIO.hpp"
#include "Graph.hpp"
#include "Point.hpp"

// DEBUG
#include <iostream>
using std::cout;
using std::endl;
// /DEBUG

// My Defines
#define REQUESTED_TEST 1
#define TREE "TREE"
#define RECURSION "RECURSION"

TerminalIO terminalIO;

int HandleGraph()
{
    int testCases = 0;
    if(terminalIO.GetNumberOfTestCases(&testCases) == -1)
    {
        terminalIO.PrintText("Invalid number of test cases");
        return -1;
    }

    for (int i = 0; i < testCases; i++)
    {

        int goal = 0;
        int connections = 0;
        if(terminalIO.GetTestParameters(&goal, &connections) == -1)
        {
            return -1;
        }

        Graph graph;
        graph.GenerateChildren(goal);

        for (int j = 0; j < connections; j++)
        {
            int sourceID = 0, destinationID = 0;
            
            if(terminalIO.GetConnection(&sourceID, &destinationID, goal) == -1)
            {
                terminalIO.PrintText("Invalid connection");
                return -1;
            }

            if(graph.AddConnection(sourceID, destinationID) == -1)
            {
                terminalIO.PrintText("Duplicate connection");
                return -1;
            }
        }

        int result = graph.Search(goal);
        terminalIO.PrintResult(result);
    }
    return 0;
}

int HandleRecursion()
{
    terminalIO.PrintText("Not implemented");
    return -1; // TODO: IMPLEMENT THIS
}

int main(int argc, char const *argv[])
{
    if(argc < REQUESTED_TEST)
    {
        terminalIO.PrintText("Not enough arguments");
        return -1;
    }
    argv = argv; // TODO: Remove this line when the code is finished.
    return HandleGraph();


    //TODO: Fix. This is not working.
    // if(strcmp(argv[REQUESTED_TEST], TREE) == 0)
    // {
    //     return HandleTree();
    // }
    // if(strcmp(argv[REQUESTED_TEST], RECURSION) == 0)
    // {
    //     return HandleRecursion();
    // }
}