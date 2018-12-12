cmake_minimum_required(VERSION 3.13)
project(CountdownWordSearch)
set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_FIND_LIBRARY_SUFFIXES .dll .lib .a .so)
SET(BUILD_SHARED_LIBS OFF)
SET(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(CountdownWordSearch main.cpp headers/countdown/Trie.h headers/countdown/TrieNode.h source/countdown/TrieNode.cpp source/countdown/Trie.cpp)