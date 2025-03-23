#include "object.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <zstr.hpp>

std::vector<std::vector<std::string>> readTreeContent(const std::string &path)
{
    // ifstream is already doing the decompression
    zstr::ifstream decompressedDataStream(path, std::ofstream::binary);
    if (!decompressedDataStream.is_open())
    {
        std::cerr << "Failed to open file " << path << '\n';
        return {};
    }

    auto treeContent = std::string(std::istreambuf_iterator<char>(decompressedDataStream), std::istreambuf_iterator<char>());
    // std::cout << "Decompressed data: " << treeContent << "\n";

    std::string trimmedData = treeContent.substr(treeContent.find('\0') + 1);
    std::vector<std::vector<std::string>> children;
    while (!trimmedData.empty())
    {
        size_t nullPos = trimmedData.find('\0');
        std::string entry = trimmedData.substr(0, nullPos);
        std::string objectHash = SHA1ToHex(trimmedData.substr(nullPos + 1, 20));
        std::string objectPath = getObjectPath(objectHash);

        // std::cout << "entry: " << entry << ", objectHash:" << objectHash << "\n";

        if (entry.substr(0, 5) == "40000")
            children.push_back({"040000", "tree", entry.substr(6), objectHash});
        else
            children.push_back({"100644", "blob", entry.substr(7), objectHash});
        trimmedData = trimmedData.substr(nullPos + 21);
    }
    sort(children.begin(), children.end(), [](const auto &a, const auto &b)
         { return a[2] < b[2]; });

    return children;
}
