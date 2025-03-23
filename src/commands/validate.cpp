#include "commands.hpp"
#include "../init/init.hpp"
#include "cat_file.hpp"
#include "hash_object.hpp"

bool validateCatFileArgs(int argc, char *argv[])
{
    if (argc <= 3)
    {
        std::cerr << "Usage: <program> cat-file -p <blob_sha>\n";
        return false;
    }

    const std::string flag = argv[2];
    if (flag != "-p")
    {
        std::cerr << "Invalid flag for cat-file, expected `-p`\n";
        return false;
    }
    return true;
}

bool validateHashObjectArgs(int argc, char *argv[])
{
    if (argc <= 2)
    {
        std::cerr << "Usage: <program> hash-object <file>\n";
        return false;
    }
    if (argc == 4)
    {
        const std::string flag = argv[2];
        if (flag != "-w")
        {
            std::cerr << "Invalid flag for hash-object, expected `-w`\n";
            return false;
        }
    }
    return true;
}

bool validateLsTreeArgs(int argc, char *argv[])
{
    if (argc <= 2)
    {
        std::cerr << "Usage: <program> ls-tree <tree_sha>\n";
        return false;
    }
    if (argc == 4)
    {
        const std::string flag = argv[2];
        if (flag != "--name-only")
        {
            std::cerr << "Invalid flag for ls-tree, expected `--name-only`\n";
            return false;
        }
    }
    return true;
}