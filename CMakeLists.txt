cmake_minimum_required(VERSION 3.17)
project(Train_Ticket)

set(CMAKE_CXX_STANDARD 14)

add_executable(Train_Ticket main.cpp BPlusTree.hpp DiskManager.hpp Myexception.hpp)