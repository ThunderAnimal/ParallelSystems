cmake_minimum_required(VERSION 3.7)
project(sub)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    core.c
    core.h
    core_test.c
    sub.c)

add_executable(sub ${SOURCE_FILES})