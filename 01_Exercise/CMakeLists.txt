cmake_minimum_required(VERSION 3.9)
project(01_Exercise)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -fopenmp)

include_directories(
        ${PROJECT_SOURCE_DIR}/../StopWatch
        ${PROJECT_SOURCE_DIR}/../FreeImage
)

set(LIBRARY_DIR  ${PROJECT_SOURCE_DIR}/DLL/x64)

find_library()

find_library(
        FREEIMAGE_LIB
        NAMES FreeImage
        PATHS ${LIBRARY_DIR}
        PATH_SUFFIXES lib
        NO_DEFAULT_PATH
)

find_library(
        FREEIMAGEPLUS_LIB
        NAMES FreeImagePlus
        PATHS ${LIBRARY_DIR}
        PATH_SUFFIXES lib
        NO_DEFAULT_PATH
)

add_executable(01_Exercise
        main.cpp
        imageprocessing.cpp
        summation.cpp)

target_link_libraries(
        01_Exercise
        ${FREEIMAGE_LIB}
        ${FREEIMAGEPLUS_LIB}
)

configure_file(${PROJECT_SOURCE_DIR}/Dll/x64/FreeImage.dll FreeImage.dll COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/Dll/x64/FreeImage.dll FreeImagePlus.dll COPYONLY)

