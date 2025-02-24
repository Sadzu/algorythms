cmake_minimum_required(VERSION 3.30)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled sources/main.cpp
        headers/Node.h
        headers/ListIterator.h
        headers/OneLinkedList.h)
