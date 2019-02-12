cmake_minimum_required(VERSION 3.13.0)

# enable C++11
enable_language(C)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)

# create project
project(TactorFX)

find_package(portaudio REQUIRED)

# include directories
include_directories(
    "include"
    "/Git/MEL/include"
    "/Program Files/portaudio/include"
)
<<<<<<< HEAD
add_executable(sine examples/paex_sine_c++.cpp)
target_link_libraries(sine portaudio_static)

#add_executable(deviceList examples/pa_devs.c)
#target_link_libraries(deviceList portaudio_static)

#add_executable(mono examples/pa_saw_mono.cpp)
#target_link_libraries(deviceList portaudio_static)
=======

link_directories(
    "/Git/MEL/build-quanser/Release"
)

add_compile_definitions(PA_USE_ASIO) # needed for devs


set(INCLUDE_FILES "include/TactorFX/ADSR.hpp")
set(SRC_FILES     "src/TactorFX/ADSR.cpp")

# make library
add_library(tfx ${INCLUDE_FILES} ${SRC_FILES})

# executables
add_executable(evan_testing tests/evan_testing.cpp)
target_link_libraries(evan_testing portaudio_static MEL)

add_executable(devs examples/devs.cpp)
target_link_libraries(devs portaudio_static)
>>>>>>> 005740cae4d30f499a401b5df02b28d3258c97bc
