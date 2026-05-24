cmake_minimum_required(VERSION 4.0)
project(b_estrella)

set(CMAKE_CXX_STANDARD 20)

add_executable(b_estrella main.cpp
        ArbolBEstrella.hpp
        ArbolBEstrella.tpp
)
