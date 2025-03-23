#include "commands.hpp"
#include "../init/init.hpp"
#include "cat_file.hpp"
#include "hash_object.hpp"

void handleInitGit()
{
    initializeGit();
}

void handleCatFile(int argc, char *argv[])
{
    if (!validateCatFileArgs(argc, argv))
    {
        return;
    }

    const std::string value = argv[3];
    const std::string path = extractBlobSHA(value);
    const std::string object_str = readObjectContent(path);

    if (!object_str.empty())
    {
        const auto object_content = object_str.substr(object_str.find('\0') + 1);
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