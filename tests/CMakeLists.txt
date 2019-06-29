# executables

add_executable(devs devs.cpp)
target_link_libraries(devs portaudio_static)

add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark syntacts)
