#include "commands.hpp"
#include "../init/init.hpp"
#include "../object/object.hpp"
#include "validate.hpp"
#include <vector>

void handleInitGit()
{
    initializeGit();
}

void handleCatFile(int argc, char *argv[])
{
    if (!validateCatFileArgs(argc, argv))
        return;

    const std::string hash = argv[3];
    const std::string path = getObjectPath(hash);
    const std::string blobContent = readBlobContent(path);

    if (!blobContent.empty())
    {
        const auto object_content = blobContent.substr(blobContent.find('\0') + 1);
        std::cout << object_content << std::flush;
    }
}

void handleHashObject(int argc, char *argv[])
{
    if (!validateHashObjectArgs(argc, argv))
        return;

    std::string filePath = argv[2];
    if (argc == 4)
        filePath = argv[3];

    const std::string flag = (argc == 4) ? argv[2] : "";
    const std::string blob = createBlob(filePath);
    const std::string hash = computeSHA1(blob);

    // std::cout << "blob: " << blob << '\n';
    std::cout << hash << '\n';

    if (flag == "-w")
    {
        // compress blob content and write to .git/objects/<hash>
        std::string compressedBlob = compressBlob(blob);
        // std::cout << "compressedBlob: " << compressedBlob << '\n';
        if (!compressedBlob.empty())
        {
            writeObjectFile(hash, compressedBlob);
        }
    }
}

void handleLsTree(int argc, char *argv[])
{
    if (!validateLsTreeArgs(argc, argv))
        return;

    std::string treeHash = argv[2];
    if (argc == 4)
        treeHash = argv[3];

    const std::string flag = (argc == 4) ? argv[2] : "";
    const std::string path = getObjectPath(treeHash);
    const std::vector<std::vector<std::string>> treeContent = readTreeContent(path);

    if (flag == "--name-only")
    {
        for (int i = 0; i < treeContent.size(); i++)
        {
            std::cout << treeContent[i][2] << "\n";
        }
    }
    else
    {
        for (int i = 0; i < treeContent.size(); i++)
        {
            std::cout << treeContent[i][0] << " " << treeContent[i][1] << " " << treeContent[i][2] << "\n";
        }
    }
}

void handleWriteTree()
{
    const std::string treeHash = createTree(".");
    std::cout << treeHash << '\n';
}