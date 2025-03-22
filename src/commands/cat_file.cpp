#include "cat_file.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <zstr.hpp>

void print_usage()
{
    std::cerr << "Usage: <program> cat-file -p <blob_sha>\n";
}

bool validate_args(int argc, char *argv[])
{
    if (argc <= 3)
    {
        print_usage();
        return false;
    }

    const std::string flag = argv[2];
    if (flag != "-p")
    {
        std::cerr << "Invalid flag for cat-file, expected `-p`\n";
        return false;
    }
    return true;
}

std::string extract_blob_sha(const std::string &value)
{
    const std::string dir_name = value.substr(0, 2);
    const std::string blob_sha = value.substr(2);
    return ".git/objects/" + dir_name + "/" + blob_sha;
}

std::string read_object_content(const std::string &path)
{
    zstr::ifstream input(path, std::ofstream::binary);
    if (!input.is_open())
    {
        std::cerr << "Failed to open file " << path << '\n';
        return "";
    }

    std::string object_str{std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
    input.close();

    return object_str;
}

void handle_cat_file(int argc, char *argv[])
{
    if (!validate_args(argc, argv))
    {
        return;
    }

    const std::string value = argv[3];
    const std::string path = extract_blob_sha(value);
    const std::string object_str = read_object_content(path);

    if (!object_str.empty())
    {
        const auto object_content = object_str.substr(object_str.find('\0') + 1);
        std::cout << object_content << std::flush;
    }
}
