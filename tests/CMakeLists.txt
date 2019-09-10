# executables
find_package(MEL REQUIRED)

add_executable(devs devs.cpp)
target_link_libraries(devs portaudio_static)

add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark syntacts)

add_executable(system_test system_test.cpp)
target_link_libraries(system_test MEL::quanser syntacts)