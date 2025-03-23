#include "ls_tree.hpp"
#include "cat_file.hpp"
#include <iostream>

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

void handleLsTree(int argc, char *argv[])
{
    if (!validateLsTreeArgs(argc, argv))
        return;

    std::string treeSHA = argv[2];
    if (argc == 4)
        treeSHA = argv[3];

    const std::string flag = (argc == 4) ? argv[2] : "";
    const std::string path = extractBlobSHA(treeSHA);
    const std::string object_str = readObjectContent(path);
}