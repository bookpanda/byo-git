#include "object.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <zstr.hpp>
#include <algorithm>

std::string extractObjectSHA(const std::string &value)
{
    const std::string dirName = value.substr(0, 2);
    const std::string blobSHA = value.substr(2);
    return ".git/objects/" + dirName + "/" + blobSHA;
}

void writeObjectFile(const std::string &hash, const std::string &compressedBlob)
{
    std::string dir = ".git/objects/" + hash.substr(0, 2);
    std::string filename = dir + "/" + hash.substr(2);

    // create the directory if it doesn't exist
    std::filesystem::create_directories(dir);

    std::ofstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Failed to write object to " << filename << '\n';
        return;
    }

    file.write(compressedBlob.data(), compressedBlob.size());
    // std::cout << "Object written to: " << filename << '\n';
}
