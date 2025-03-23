#include "object.hpp"
#include <iostream>
#include <openssl/evp.h>
#include <iomanip>
#include <zstr.hpp>

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
