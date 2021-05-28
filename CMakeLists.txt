cmake_minimum_required(VERSION 3.16)
project(code)
add_compile_options(-std=c++11 -O2)
set(CMAKE_CXX_STANDARD 14)

add_executable(code main.cpp DataStructure/BPlusTree.hpp DataStructure/DiskManager.hpp DataStructure/Myexception.hpp DataStructure/functions.hpp DataStructure/vector.hpp DataStructure/HashMap.hpp all_header.cpp Engine.cpp DataStructure/DiskStack.hpp)