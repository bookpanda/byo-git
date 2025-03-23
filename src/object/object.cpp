#include "object.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <zstr.hpp>
#include <algorithm>

std::string extractObjectSHA(const std::string &value)
{
    const std::string dirName = value.substr(0, 2);
    const std::string blobSHA = value.substr(2);
    return ".git/objects/" + dirName + "/" + blobSHA;
}
