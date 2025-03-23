#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>

// object
std::string getObjectPath(const std::string &hash);
void writeObjectFile(const std::string &hash, const std::string &compressedBlob);

// blob
std::string readBlobContent(const std::string &path);
std::string createBlob(const std::string &filePath);

// tree
std::string readTreeContent(const std::string &path);

// compress
std::string decompressBlob(const std::string &compressedData);
std::string compressBlob(const std::string &blob);

// sha
std::string computeSHA1(const std::string &blob);

#endif // OBJECT_H