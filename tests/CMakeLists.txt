# executables
find_package(MEL REQUIRED)

add_executable(devs devs.cpp)
target_link_libraries(devs portaudio_static)

add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark syntacts)

# add_executable(type_erasure type_erasure.cpp)
# target_link_libraries(type_erasure syntacts)

add_executable(clone clone.cpp)
target_link_libraries(clone syntacts)

add_executable(exp_api exp_api.cpp)
target_link_libraries(exp_api syntacts)

# add_executable(system_test system_test.cpp)
# target_link_libraries(system_test MEL::quanser syntacts)
