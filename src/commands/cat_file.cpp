#include "cat_file.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <zstr.hpp>
#include <algorithm>

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

std::string readObjectContent(const std::string &path)
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

std::vector<std::string> splitStringByNullByte(const std::string &str)
{
    std::vector<std::string> result;
    size_t start = 0;
    size_t end;
    while ((end = str.find('\0', start)) != std::string::npos)
    {
        result.push_back(str.substr(start, end - start));
        start = end + 1;
    }
    return result;
}
