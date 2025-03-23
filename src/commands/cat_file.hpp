#ifndef GIT_CAT_FILE_H
#define GIT_CAT_FILE_H

#include <iostream>

void handleCatFile(int argc, char *argv[]);

std::string extractBlobSHA(const std::string &value);

std::string readObjectContent(const std::string &path);

#endif // GIT_CAT_FILE_H
