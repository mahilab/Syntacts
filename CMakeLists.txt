cmake_minimum_required(VERSION 3.13.0)

enable_language(C)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)

# create project
project(TactorFX)

find_package(portaudio REQUIRED)

# include directories
include_directories(
    "include"
    "C:/Program Files/portaudio/include"
)
add_executable(tfx examples/paex_saw.cpp)
target_link_libraries(tfx portaudio_static)

