#include "hash_object.hpp"
#include <iostream>
#include <zstr.hpp>
#include <openssl/evp.h>
#include <iomanip>
#include <filesystem>

bool validateHashObjectArgs(int argc, char *argv[])
{
    if (argc <= 2)
    {
        std::cerr << "Usage: <program> hash-object <file>\n";
        return false;
    }
    if (argc == 4)
    {
        const std::string flag = argv[2];
        if (flag != "-w")
        {
            std::cerr << "Invalid flag for hash-object, expected `-w`\n";
            return false;
        }
    }
    return true;
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

std::string computeSHA1(const std::string &blob)
{
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (!mdctx)
    {
        std::cerr << "Failed to create EVP_MD_CTX\n";
        return "";
    }

    if (EVP_DigestInit_ex(mdctx, EVP_sha1(), nullptr) != 1 ||
        EVP_DigestUpdate(mdctx, blob.data(), blob.size()) != 1) // processes the data in chunks, allowing you to hash large inputs incrementally
    {
        std::cerr << "SHA1 computation failed\n";
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    // computed sha1 hash here as 20-byte binary hash
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;
    if (EVP_DigestFinal_ex(mdctx, hash, &hashLen) != 1) // finalizes the computation and stores the result in hash
    {
        std::cerr << "SHA1 computation failed\n";
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    EVP_MD_CTX_free(mdctx); // cleanup

    // {0x3a, 0x5b, 0xff, 0x10, ...} => "3a5bff10..."
    std::ostringstream hash_str;
    for (unsigned int i = 0; i < hashLen; i++) // convert hash into 40-character hex string
    {
        hash_str << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return hash_str.str();
}

std::string compressBlob(const std::string &blob)
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

void handleHashObject(int argc, char *argv[])
{
    if (!validateHashObjectArgs(argc, argv))
        return;

    std::string filePath = argv[2];
    if (argc == 4)
        filePath = argv[3];

    const std::string flag = (argc == 4) ? argv[2] : "";
    const std::string blob = createBlob(filePath);
    const std::string hash = computeSHA1(blob);

    // std::cout << "blob: " << blob << '\n';
    std::cout << hash << '\n';

    if (flag == "-w")
    {
        // compress blob content and write to .git/objects/<hash>
        std::string compressedBlob = compressBlob(blob);
        std::cout << "compressedBlob: " << compressedBlob << '\n';
        if (!compressedBlob.empty())
        {
            writeObjectFile(hash, compressedBlob);
        }
    }
}
