cmake_minimum_required(VERSION 3.13.0)
project(MySyntactsProject VERSION 1.0.0)

# OPTION 1: find_package - use if you built and installed Syntacts to your system using CMake

find_package(syntacts REQUIRED)

# OPTION 2: FetchContent - use if you did not installed Syntacts and/or want to build an inplace copy

# include(FetchContent) 
# set(SYNTACTS_BUILD_GUI OFF CACHE BOOL "" FORCE)
# set(SYNTACTS_BUILD_C_DLL OFF CACHE BOOL "" FORCE)
# set(SYNTACTS_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
# set(SYNTACTS_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# FetchContent_Declare(syntacts GIT_REPOSITORY https://github.com/mahilab/Syntacts.git)
# FetchContent_MakeAvailable(syntacts)

add_executable(myApp main.cpp)
target_link_libraries(myApp syntacts)