#ifndef GIT_HASH_OBJECT_H
#define GIT_HASH_OBJECT_H

#include <iostream>

bool validateHashObjectArgs(int argc, char *argv[]);
std::string createBlob(const std::string &filePath);
std::string computeSHA1(const std::string &blob);
std::string compressBlob(const std::string &blob);
void writeObjectFile(const std::string &hash, const std::string &compressedBlob);

#endif // GIT_HASH_OBJECT_H