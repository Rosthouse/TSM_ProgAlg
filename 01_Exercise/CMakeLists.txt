cmake_minimum_required(VERSION 3.9)
project(01_Exercise)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -fopenmp)

include_directories(
        ${PROJECT_SOURCE_DIR}/../StopWatch
        ${PROJECT_SOURCE_DIR}/../FreeImage
)


SET(CMAKE_FIND_LIBRARY_PREFIXES "")
SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
set(LIBRARY_DIR ${CMAKE_HOME_DIRECTORY}/DLL/x64)

message("LIBRARY_DIR:" ${LIBRARY_DIR})
message("BINARY:" ${CMAKE_BINARY_DIR})


find_library(FREEIMAGE_LIB NAMES FreeImage PATHS ${LIBRARY_DIR})

find_library(FREEIMAGEPLUS_LIB NAMES FreeImagePlus PATHS ${LIBRARY_DIR})


message("FreeImage:" ${FREEIMAGE_LIB})
message("FreeImagePlus:" ${FREEIMAGEPLUS_LIB})


add_executable(01_Exercise
        main.cpp
        imageprocessing.cpp
        summation.cpp)

target_link_libraries(
        01_Exercise
        ${FREEIMAGE_LIB}
        ${FREEIMAGEPLUS_LIB}
)

configure_file(${LIBRARY_DIR}/FreeImage.dll FreeImage.dll COPYONLY)
configure_file(${LIBRARY_DIR}/FreeImage.dll FreeImagePlus.dll COPYONLY)

