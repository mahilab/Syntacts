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

link_directories(
    "/Git/MEL/build/Release"
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

add_executable(saw_mono examples/pa_saw_mono.cpp)
target_link_libraries(saw_mono portaudio_static)

add_executable(six_channel_testing_2 tests/six_channel_testing_2.cpp)
target_link_libraries(six_channel_testing_2 portaudio_static MEL)

add_executable(two_channel_testing tests/two_channel_testing.cpp)
target_link_libraries(two_channel_testing portaudio_static MEL)

add_executable(two_channel_testing_time tests/two_channel_testing_time.cpp)
target_link_libraries(two_channel_testing_time portaudio_static MEL)

add_executable(sine_cxx examples/paex_sine_c++.cpp)
target_link_libraries(sine_cxx portaudio_static MEL)

add_executable(sine tests/paex_sine.c)
target_link_libraries(sine portaudio_static MEL)

add_executable(saw_six_ch tests/saw_six_ch.cpp)
target_link_libraries(saw_six_ch portaudio_static MEL)

add_executable(sine_six tests/sine_six.cpp)
target_link_libraries(sine_six portaudio_static MEL)

add_executable(sine_six_ASR tests/sine_six_ASR.cpp)
target_link_libraries(sine_six_ASR portaudio_static MEL)

add_executable(saw_six_ASR tests/saw_six_ASR.cpp)
target_link_libraries(saw_six_ASR portaudio_static MEL)

add_executable(christiansen tests/Christiansen_six.cpp)
target_link_libraries(christiansen portaudio_static MEL)

add_executable(double_sin_six tests/double_sin_six.cpp)
target_link_libraries(double_sin_six portaudio_static MEL)