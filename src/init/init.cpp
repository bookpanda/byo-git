#include "init.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>

void initializeGit()
{
    try
    {
        std::filesystem::create_directory(".git");
        std::filesystem::create_directory(".git/objects");
        std::filesystem::create_directory(".git/refs");

        std::ofstream headFile(".git/HEAD");
        if (headFile.is_open())
        {
            headFile << "ref: refs/heads/main\n";
            headFile.close();
        }
        else
        {
            std::cerr << "Failed to create .git/HEAD file.\n";
            exit(EXIT_FAILURE);
        }

        std::cout << "Initialized git directory\n";
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << e.what() << '\n';
        exit(EXIT_FAILURE);
    }
}
