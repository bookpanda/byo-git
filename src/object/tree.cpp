#include "object.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <zstr.hpp>

std::string readTreeContent(const std::string &path)
{
    // ifstream is already doing the decompression
    // zstr::ifstream input(path, std::ofstream::binary);
    auto input = std::ifstream(path);
    if (!input.is_open())
    {
        std::cerr << "Failed to open file " << path << '\n';
        return "";
    }

    auto tree_data = std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
    auto uncompressedData = decompressBlob(tree_data);

    // std::string trimmed_data = tree_data.substr(tree_data.find('\0') + 1);
    std::string trimmed_data = uncompressedData.substr(uncompressedData.find('\0') + 1);
    std::string line;
    std::vector<std::string> names;
    do
    {

        line = trimmed_data.substr(0, trimmed_data.find('\0'));
        std::cout << "line: " << line << "\n";
        if (line.substr(0, 5) == "40000")
            names.push_back(line.substr(6));
        else
            names.push_back(line.substr(7));
        trimmed_data = trimmed_data.substr(trimmed_data.find('\0') + 21);
    } while (trimmed_data.size() > 1);
    sort(names.begin(), names.end());
    for (int i = 0; i < names.size(); i++)
    {
        std::cout << names[i] << "\n";
    }

    std::cout << "Decompressed data: " << tree_data << "\n";
    return tree_data;

    // std::string compressedData{std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
    // input.close();
    // std::cout << "compressedData: " << compressedData << '\n';

    // return (compressedData);
}
