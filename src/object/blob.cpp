#include "object.hpp"
#include <iostream>
#include <zstr.hpp>

std::string readBlobContent(const std::string &path)
{
    // ifstream is already doing the decompression
    zstr::ifstream input(path, std::ofstream::binary);
    // auto input = std::ifstream(path);
    if (!input.is_open())
    {
        std::cerr << "Failed to open file " << path << '\n';
        return "";
    }

    auto tree_data = std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
    // auto uncompressedData = decompressBlob(tree_data);

    return tree_data;
}