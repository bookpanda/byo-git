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

std::string createBlob(const std::string &filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file " << filePath << '\n';
        return "";
    }

    std::ostringstream contentStream;
    char buffer[8192]; // read file in chunks (8KB = file system block size)
    // file.gcount() returns the number of characters read by the last read operation.
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0)
    {
        contentStream.write(buffer, file.gcount()); // process only file.gcount() bytes
    }

    std::string content = contentStream.str();

    // Blob format: "blob <size>\0<content>"
    std::ostringstream blob_stream;
    blob_stream << "blob ";
    blob_stream << content.size();
    blob_stream.put('\0');
    blob_stream.write(content.data(), content.size());

    return blob_stream.str();
}
