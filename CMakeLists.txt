cmake_minimum_required(VERSION 3.15.2)

set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-9")
#set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2")

project(DII_STT VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory(src)