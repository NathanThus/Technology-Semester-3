// System Includes
// My Includes
#include "TerminalIO.hpp"
#include "Graph.hpp"
#include "Point.hpp"

#include "Tree.hpp"

// DEBUG
#include <iostream>
using std::cout;
using std::endl;
// /DEBUG

// My Defines
#define REQUESTED_TEST 1
#define GRAPH "GRAPH"
#define RECURSION "RECURSION"

TerminalIO terminalIO;

int HandleGraph()
{
    int testCases = 0;
    if(terminalIO.GetNumberOfTestCases(testCases) == -1)
    {
        terminalIO.PrintText("Invalid number of test cases");
        return -1;
    }

    for (int i = 0; i < testCases; i++)
    {

        int goal = 0;
        int connections = 0;
        if(terminalIO.GetTestParameters_Graph(goal, connections) == -1)
        {
            return -1;
        }

        Graph graph;
        graph.GenerateChildren(goal);

        for (int j = 0; j < connections; j++)
        {
            int sourceID = 0, destinationID = 0;
            
            if(terminalIO.GetGraphConnection(sourceID, destinationID, goal) == -1)
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
    int numberOfElements;
    if( terminalIO.GetNumberOfElements(numberOfElements) == -1)
    {
        terminalIO.PrintText("Invalid number of elements");
        return -1;
    }

    Tree tree;
    tree.GenerateBranches(numberOfElements);
    int result = -1;
    if(tree.DeepestPath(result) == -1)
    {
        terminalIO.PrintText("Invalid tree");
        return -1;
    }
    
    terminalIO.PrintResult(result);

    return 0;
}

int main(int argc, char const *argv[])
{
    if(argc < REQUESTED_TEST)
    {
        terminalIO.PrintText("Not enough arguments");
        return -1;
    }

    if(argv[REQUESTED_TEST] == nullptr)
    {
        terminalIO.PrintText("Invalid argument");
        return -1;
    }

    if(strcmp(argv[REQUESTED_TEST], GRAPH) == 0)
    {
        return HandleGraph();
    }
    
    if(strcmp(argv[REQUESTED_TEST], RECURSION) == 0)
    {
        return HandleRecursion();
    }
}