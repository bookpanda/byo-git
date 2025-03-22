cmake_minimum_required(VERSION 3.13)

project(git-starter-cpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard

# Set VCPKG toolchain if you're using vcpkg
if (EXISTS "${CMAKE_SOURCE_DIR}/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg.cmake" CACHE STRING "VCPKG Toolchain" FORCE)
endif()

# Find packages via vcpkg
find_package(ZLIB REQUIRED)

# Find zstr package (header-only)
find_path(ZSTR_INCLUDE_DIRS "zstr.hpp")
if (NOT ZSTR_INCLUDE_DIRS)
    message(FATAL_ERROR "Could not find zstr.hpp")
endif()

# Include directories
include_directories(${ZSTR_INCLUDE_DIRS})

# Get all source files
file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

# Add executable
add_executable(git ${SOURCE_FILES})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
target_link_libraries(git PRIVATE ZLIB::ZLIB OpenSSL::Crypto)


# Link the zstr headers
target_include_directories(git PRIVATE ${ZSTR_INCLUDE_DIRS})

# cmake_minimum_required(VERSION 3.13)

# project(git-starter-cpp)

# set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard

# file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

# add_executable(git ${SOURCE_FILES})

# target_link_libraries(git -lz)
