#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>

// object
std::string extractObjectSHA(const std::string &value);

// blob
std::string readBlobContent(const std::string &path);
std::string createBlob(const std::string &filePath);

// tree
std::string readTreeContent(const std::string &path);

std::string computeSHA1(const std::string &blob);
std::string decompressBlob(const std::string &compressedData);
std::string compressBlob(const std::string &blob);
void writeObjectFile(const std::string &hash, const std::string &compressedBlob);

#endif // OBJECT_H