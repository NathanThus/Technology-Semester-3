#include <stdio.h>

class TerminalIO
{
private:
    /* data */
public:
    TerminalIO();
    ~TerminalIO();

    // If these are called in this order, you should read the file properly!
    
    /// @brief  Get the number of test cases from the file stream
    /// @param stream The file stream to read from
    /// @return Returns the number of test cases
    int GetNumberOfTestCases(FILE* stream);
    /// @brief Get the number of nodes from the file stream
    /// @param stream The file stream to read from
    /// @return Returns the number of nodes
    int GetNumberOfNodes(FILE* stream);

    /// @brief Get the number of connections from the file stream
    /// @param stream The file stream to read from
    /// @return Returns the number of connections
    int GetNumberOfConnections(FILE* stream);

    /// @brief Prints the result to the file stream
    /// @param stream The file stream to write to
    /// @param result The result (array of integers)
    /// @param numberOfTestCases The number of test cases
    void PrintResult(FILE* stream, int *result, int numberOfTestCases);
};
