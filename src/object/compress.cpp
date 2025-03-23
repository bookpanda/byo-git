#include "object.hpp"
#include <iostream>
#include <zlib.h>
#include <zstr.hpp>

std::string decompressObject(const std::string &compressedData)
{
    std::string buf;
    buf.resize(compressedData.size());
    while (true)
    {
        auto buf_size = buf.size();
        auto res = uncompress(reinterpret_cast<Bytef *>(buf.data()), &buf_size, reinterpret_cast<const Bytef *>(compressedData.data()), compressedData.size());
        if (res == Z_BUF_ERROR)
        {
            buf.resize(buf.size() * 2);
        }
        else if (res != Z_OK)
        {
            std::cerr << "Failed to decompress file, code:" << res << "\n";
            return "";
        }
        else
        {
            buf.resize(buf_size);
            break;
        }
    }

    return buf;
}

std::string compressObject(const std::string &blob)
{
    uLongf compressedSize = compressBound(blob.size()); // upper bound on compressed size
    std::vector<Bytef> compressedData(compressedSize);
    if (compress2(compressedData.data(), &compressedSize, (const Bytef *)blob.data(), blob.size(), Z_BEST_COMPRESSION) != Z_OK)
    {
        std::cerr << "Failed to compress blob content\n";
        return "";
    }

    // construct a string from the compressed data
    return std::string(reinterpret_cast<char *>(compressedData.data()), compressedSize);
}