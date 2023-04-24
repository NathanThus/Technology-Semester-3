#include <stdio.h>

class TerminalIO
{
private:
    /* data */
public:
    TerminalIO(/* args */);
    ~TerminalIO();
    int GetNumberOfTestCases(FILE* stream);
    int GetNumberOfNodes(FILE* stream);
    int GetNumberOfConnections(FILE* stream);
    
    void PrintResult(int* result, int numberOfTestCases);
};
