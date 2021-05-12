cmake_minimum_required(VERSION 3.16)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code main.cpp DataStructure/BPlusTree.hpp DataStructure/DiskManager.hpp DataStructure/Myexception.hpp DataStructure/functions.hpp DataStructure/vector.hpp DataStructure/HashMap.hpp all_header.cpp Engine.cpp)