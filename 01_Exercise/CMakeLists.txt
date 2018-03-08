cmake_minimum_required(VERSION 3.9)
project(01_Exercise)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-fopenmp")

add_executable(01_Exercise
        main.cpp
        imageprocessing.cpp
        summation.cpp)