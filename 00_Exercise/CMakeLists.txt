cmake_minimum_required(VERSION 2.8.4)
project(00_Exercise)

set(CMAKE_CXX_STANDARD 11)

add_executable(00_Exercise
        BankAccount.h
        main.cpp
        RWLock.h)
