cmake_minimum_required(VERSION 2.8.4)
project(02_Exercise)

set(CMAKE_CXX_STANDARD 14)
set(ENV{FREEIMAGE_SDK} "${PROJECT_SOURCE_DIR}/../FreeImage" CACHE PATH)
set(ENV{STOPWATCH} "${PROJECT_SOURCE_DIR}/../Stopwatch" CACHE PATH)
set(ENV{OPEN_CL} "F:/SDKs/OpenCL/OpenCL-Headers/opencl22" CACHE PATH)

find_package(OpenCL REQUIRED)

set(
        SOURCE_FILES
        main.h
        main.cpp
        acc.cpp
        cl.hpp
        ocl.h
        ocl.cpp
)

add_library(FREEIMAGE_LIB SHARED IMPORTED)
add_library(FREEIMAGEPLUS_LIB SHARED IMPORTED)
add_library(OPEN_CL_LIB SHARED IMPORTED)

set_property(TARGET FREEIMAGE_LIB PROPERTY IMPORTED_LOCATION $ENV{FREEIMAGE_SDK}/Win32/FreeImage.dll)
set_property(TARGET FREEIMAGE_LIB PROPERTY IMPORTED_IMPLIB $ENV{FREEIMAGE_SDK}/Win32/FreeImage.lib)
configure_file($ENV{FREEIMAGE_SDK}/Win32/FreeImage.dll  ${PROJECT_BINARY_DIR}/Debug/FreeImage.dll COPYONLY)

set_property(TARGET FREEIMAGEPLUS_LIB PROPERTY IMPORTED_LOCATION $ENV{FREEIMAGE_SDK}/Win32/FreeImagePlus.dll)
set_property(TARGET FREEIMAGEPLUS_LIB PROPERTY IMPORTED_IMPLIB $ENV{FREEIMAGE_SDK}/Win32/FreeImagePlus.lib)
configure_file($ENV{FREEIMAGE_SDK}/Win32/FreeImagePlus.dll  ${PROJECT_BINARY_DIR}/Debug/FreeImagePlus.dll COPYONLY)

set(INCLUDE_DIRECTORIES
        $ENV{FREEIMAGE_SDK}
        $ENV{STOPWATCH}
        $ENV{OPEN_CL})

set(WIN32 1)

option(_WIN32 "Win32" ON)
option(WIN32 "Win32" ON)

if (_WIN32)
    add_definitions(-D_WIN32)
endif ()
if (WIN32)
    add_definitions(-DWIN32)
endif ()


if (MSVC)
    message("Compiling for MSVC")
    set(SOURCE_FILES ${SOURCE_FILES} amp.cpp)
else (CYGWIN)
    set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} $ENV{OPEN_CL})
endif ()

set(RESOURCE_FILES
  edges.cl
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/edges.cl ${PROJECT_BINARY_DIR}/Debug/edges.cl COPYONLY)

include_directories(
        ${INCLUDE_DIRECTORIES}
)

add_executable(02_Exercise ${SOURCE_FILES} ${RESOURCE_FILES})
target_link_libraries(02_Exercise FREEIMAGE_LIB FREEIMAGEPLUS_LIB OpenCL::OpenCL)