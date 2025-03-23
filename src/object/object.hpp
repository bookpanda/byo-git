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
std::string createTree(const std::string &path, int depth = 0);
std::string commitTree(const std::string treeSHA, const std::string parentCommitHash, const std::string message);

// compress
std::string decompressObject(const std::string &compressedData);
std::string compressObject(const std::string &blob);

// sha
std::string computeSHA1(const std::string &blob);
std::string SHA1ToHex(const std::string &binarySha);
std::string hexToSHA1(const std::string &hex);

#endif // OBJECT_H