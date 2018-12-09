cmake_minimum_required(VERSION 3.13)
project(CountdownWordSearch)
set(CMAKE_CXX_STANDARD 17)

configure_file(resources/words.txt resources/words.txt COPYONLY)

add_executable(CountdownWordSearch main.cpp headers/countdown/Trie.h headers/countdown/TrieNode.h)