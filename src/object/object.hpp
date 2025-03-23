#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>

// object
std::string getObjectPath(const std::string &hash);
void writeObjectFile(const std::string &hash, const std::string &compressedBlob);

// blob
std::string readBlobContent(const std::string &path);
std::string createBlob(const std::string &filePath);

// tree
std::vector<std::vector<std::string>> readTreeContent(const std::string &path);

// compress
std::string decompressBlob(const std::string &compressedData);
std::string compressBlob(const std::string &blob);

// sha
std::string computeSHA1(const std::string &blob);
std::string SHA1ToHex(const std::string &binarySha);

#endif // OBJECT_H