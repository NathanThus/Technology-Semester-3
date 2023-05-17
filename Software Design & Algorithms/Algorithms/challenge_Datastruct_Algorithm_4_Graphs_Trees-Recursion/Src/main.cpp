// System Includes

// My Includes

// My Defines

#define EXPECTED_ARGC 1

int main(int argc, char const *argv[])
{
    if(argc != EXPECTED_ARGC)
    {
        return -1;
    }

    if(argv == nullptr)
    {
        return -1;
    }

    // Arg = 0...
    // Arg = 1...

    return 0;
}
