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
add_executable(sine examples/paex_sine_c++.cpp)
target_link_libraries(sine portaudio_static)

#add_executable(deviceList examples/pa_devs.c)
#target_link_libraries(deviceList portaudio_static)

#add_executable(mono examples/pa_saw_mono.cpp)
#target_link_libraries(deviceList portaudio_static)