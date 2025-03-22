#include "hash_object.hpp"
#include <iostream>
#include <zstr.hpp>
#include <openssl/sha.h>

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
    char buffer[8192]; // Read file in chunks
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0)
    {
        contentStream.write(buffer, file.gcount());
    }

    std::string content = contentStream.str();
}

std::string computeSHA1(const std::string &filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file " << filePath << '\n';
        return "";
    }

    SHA_CTX sha1; // init sha1 context
    SHA1_Init(&sha1);

    char buffer[8192]; // read file in chunks (8KB = file system block size)
    // file.gcount() returns the number of characters read by the last read operation.
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0)
    {
        SHA1_Update(&sha1, buffer, file.gcount()); // process only file.gcount() bytes
    }

    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1_Final(hash, &sha1); // computed sha1 hash here as 20-byte binary hash

    // {0x3a, 0x5b, 0xff, 0x10, ...} => "3a5bff10..."
    std::ostringstream hash_str;
    for (unsigned char c : hash) // convert hash into 40-character hex string
    {
        hash_str << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    }

    return hash_str.str();
}

void handleHashObject(int argc, char *argv[])
{
    if (!validateHashObjectArgs(argc, argv))
        return;

    std::string filePath = argv[2];
    if (argc == 4)
        filePath = argv[3];

    const std::string flag = (argc == 4) ? argv[2] : "";
    const std::string hash = computeSHA1(filePath);

    std::cout << hash << '\n';

    if (flag == "-w")
    {
    }
    else
    {
    }
}
