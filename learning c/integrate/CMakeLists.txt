make_minimum_required(VERSION 3.7)
project(integrate)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c integrate.c integrate.h)
add_executable(integrate ${SOURCE_FILES})