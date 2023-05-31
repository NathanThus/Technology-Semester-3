#ifndef TERMINALIO_HPP
#define TERMINALIO_HPP

#include <string>
#include <cstring>

enum MENU_CHOICE
{
    TREE,
    RECURSION,
    INVALID
};

class TerminalIO
{
    private:
        /* data */
    public:
        TerminalIO() = default;
        ~TerminalIO() = default;


    int GetNumberOfTestCases(int& numberOfTestCases);
    int GetNumberOfElements(int& numberOfElements);
    int GetTestParameters_Graph(int& goal, int& connections);
    int GetGraphConnection(int& source, int& destination, int goal);
    int GetTreeConnection(int& source, int& left, int& right);
    void PrintResult(int result);
    void PrintText(std::string error);
    MENU_CHOICE GetMenuChoice(const char* menuChoice);

};

#endif // TERMINALIO_HPP
