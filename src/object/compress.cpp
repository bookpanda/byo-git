#include "object.hpp"
#include <iostream>
#include <zlib.h>

std::string decompressBlob(const std::string &compressedData)
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