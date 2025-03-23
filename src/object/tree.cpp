#include "object.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <zstr.hpp>
#include <set>

namespace fs = std::filesystem;

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

std::string createTree(const std::string &path, int depth)
{
    try
    {
        std::set<fs::directory_entry> sorted_by_name;
        for (const auto &entry : fs::directory_iterator(path))
        {
            sorted_by_name.insert(entry);
        }

        int treeSize = 0;
        std::vector<std::vector<std::string>> children;
        for (const auto &entry : sorted_by_name)
        {
            std::string entryPath = entry.path().string();
            std::string entryName = entry.path().filename().string();

            if (entryName == ".git")
                continue;

            // std::cout << std::string(depth * 2, ' ') // Indentation for depth
            //           << (entry.is_directory() ? "[DIR]  " : "[FILE] ")
            //           << entryName << ", namesize: " << entryName.size() << "\n";

            if (entry.is_directory())
            {
                auto hash = createTree(entryPath, depth + 1);
                children.push_back({"tree", entryName, hexToSHA1(hash)});
                treeSize += 5 + 1 + entryName.size() + 1 + 20;
            }
            else
            {
                auto blobContent = createBlob(entryPath);
                auto hash = computeSHA1(blobContent);
                writeObjectFile(hash, compressObject(blobContent));

                children.push_back({"blob", entryName, hexToSHA1(hash)});
                treeSize += 6 + 1 + entryName.size() + 1 + 20;
                // mode, space, name, \0, SHA
            }
        }

        std::string treeContent;
        for (const auto &child : children)
        {
            if (child[0] == "tree")
                treeContent += "40000 " + child[1] + '\0' + child[2];
            else
                treeContent += "100644 " + child[1] + '\0' + child[2];
        }

        treeContent = "tree " + std::to_string(treeSize) + '\0' + treeContent;
        auto treeHash = computeSHA1(treeContent);
        writeObjectFile(treeHash, compressObject(treeContent));
        // std::cout << "treeContent: " << treeContent << "\n";

        return treeHash;
    }
    catch (const fs::filesystem_error &err)
    {
        std::cerr << "Error accessing path: " << err.what() << '\n';
        return "";
    }
}