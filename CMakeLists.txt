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
    "C:/Program Files/portaudio/include"
)

add_compile_definitions(PA_USE_ASIO) # needed for devs

# executables
add_executable(waveforms examples/waveforms.cpp)
target_link_libraries(waveforms portaudio_static)

add_executable(evan examples/evan.cpp)
target_link_libraries(evan portaudio_static)

add_executable(devs examples/devs.cpp)
target_link_libraries(devs portaudio_static)