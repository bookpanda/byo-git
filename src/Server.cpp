#include <iostream>
#include "./commands/commands.hpp"

int main(int argc, char *argv[])
{
    // Flush after every std::cout / std::cerr i.e. output will be printed on console immediately.
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cerr << "Logs from your program will appear here!\n";

    // Uncomment this block to pass the first stage
    if (argc < 2)
    {
        std::cerr << "No command provided.\n";
        return EXIT_FAILURE;
    }

    std::string command = argv[1];

    if (command == "init")
    {
        handleInitGit();
    }
    else if (command == "cat-file")
    {
        handleCatFile(argc, argv);
    }
    else if (command == "hash-object")
    {
        handleHashObject(argc, argv);
    }
    else
    {
        std::cerr << "Unknown command " << command << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
