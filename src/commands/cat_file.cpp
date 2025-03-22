#include "cat_file.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <zstr.hpp>

void printUsage()
{
    std::cerr << "Usage: <program> cat-file -p <blob_sha>\n";
}

bool validateCatFileArgs(int argc, char *argv[])
{
    if (argc <= 3)
    {
        printUsage();
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

std::string extractBlobSHA(const std::string &value)
{
    const std::string dirName = value.substr(0, 2);
    const std::string blobSHA = value.substr(2);
    return ".git/objects/" + dirName + "/" + blobSHA;
}

std::string readObjectContent(const std::string &path)
{
    zstr::ifstream input(path, std::ofstream::binary);
    if (!input.is_open())
    {
        std::cerr << "Failed to open file " << path << '\n';
        return "";
    }

    std::string object_str{std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
    input.close();

    return object_str;
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
