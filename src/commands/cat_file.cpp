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
    z_stream stream{};
    stream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(compressedData.data()));
    stream.avail_in = compressedData.size();

    if (inflateInit(&stream) != Z_OK)
    {
        std::cerr << "Failed to initialize zlib decompression\n";
        return "";
    }

    std::vector<char> decompressedData(8192); // Initial buffer (8KB)
    std::string result;

    int ret;
    do
    {
        stream.next_out = reinterpret_cast<Bytef *>(decompressedData.data());
        stream.avail_out = decompressedData.size();

        ret = inflate(&stream, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR)
        {
            std::cerr << "zlib decompression error: " << ret << '\n';
            inflateEnd(&stream);
            return "";
        }

        result.append(decompressedData.data(), decompressedData.size() - stream.avail_out);
    } while (ret != Z_STREAM_END); // Continue until full decompression

    inflateEnd(&stream);
    return result;
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
    // std::string buf;
    // buf.resize(tree_data.size());
    // while (true)
    // {
    //     auto buf_size = buf.size();
    //     auto res = uncompress(reinterpret_cast<Bytef *>(buf.data()), &buf_size, reinterpret_cast<const Bytef *>(tree_data.data()), tree_data.size());
    //     if (res == Z_BUF_ERROR)
    //     {
    //         buf.resize(buf.size() * 2);
    //     }
    //     else if (res != Z_OK)
    //     {
    //         std::cerr << "Failed to decompress file, code:" << res << "\n";
    //         return "";
    //     }
    //     else
    //     {
    //         buf.resize(buf_size);
    //         break;
    //     }
    // }

    std::string trimmed_data = tree_data.substr(tree_data.find('\0') + 1);
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

void printTreeContents(const std::string &treeHash)
{
    const std::string path = extractBlobSHA(treeHash);
    const std::string treeContent = readObjectContent(path);
    // const std::string decompressedContent = decompressBlob(treeContent);

    std::vector<std::string> treeEntries = splitStringByNullByte(treeContent);

    for (const auto &entry : treeEntries)
    {
        std::cout << "Entry: " << entry << "\n";
        std::istringstream entryStream(entry);
        std::string objectHash, permission, fileName;
        entryStream >> objectHash >> permission >> fileName;

        if (permission == "100644") // Blob (file)
        {
            // Handle the file (blob) content
            std::cout << "File: " << fileName << " - Object hash: " << objectHash << "\n";
            const std::string fileContent = readObjectContent(".git/objects/" + objectHash.substr(0, 2) + "/" + objectHash.substr(2));
            const std::string decompressedFileContent = decompressBlob(fileContent);
            std::cout << "File Content:\n"
                      << decompressedFileContent << "\n";
        }
        else if (permission == "40000") // Tree (subdirectory)
        {
            // Handle the subtree (recursively print the contents of the subtree)
            std::cout << "Subtree: " << fileName << " - Object hash: " << objectHash << "\n";
            printTreeContents(objectHash);
        }
    }
}

void handleCatFile(int argc, char *argv[])
{
    if (!validateCatFileArgs(argc, argv))
    {
        return;
    }

    const std::string value = argv[3];
    const std::string path = extractBlobSHA(value);
    // printTreeContents(value);
    const std::string object_str = readObjectContent(path);

    // if (!object_str.empty())
    // {
    //     const auto object_content = object_str.substr(object_str.find('\0') + 1);
    //     std::cout << object_content << std::flush;
    // }
}
