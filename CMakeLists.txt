cmake_minimum_required(VERSION 3.13.0)

enable_language(C)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)

# create project
project(TactorFX)

# include directories
include_directories(
    "include"
)

add_executable(tfx examples/example1.cpp)

