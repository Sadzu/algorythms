cmake_minimum_required(VERSION 3.30)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled sources/main.cpp
        headers/Graph.h
        headers/ShortestPathTask.h
        headers/VertexDescriptor.h
        headers/EdgeDescriptor.h
        headers/SimpleGraph.h
        headers/GraphStructure.h
        headers/LGraph.h
        headers/Mgraph.h
)
