# executables
# find_package(MEL REQUIRED)

add_executable(devs devs.cpp)
target_link_libraries(devs portaudio_static)

# add_executable(benchmark benchmark.cpp)
# target_link_libraries(benchmark syntacts)

# # add_executable(type_erasure type_erasure.cpp)
# # target_link_libraries(type_erasure syntacts)

# add_executable(clone clone.cpp)
# target_link_libraries(clone syntacts)

# add_executable(exp_api exp_api.cpp)
# target_link_libraries(exp_api syntacts)

# add_executable(testLoop testLoop.cpp)
# target_link_libraries(testLoop MEL::quanser syntacts)

add_executable(serial1 serial1.cpp)
target_include_directories(serial1 PUBLIC "../3rdparty/")
target_link_libraries(serial1 carnot)

add_library(serial_lib serial_lib.hpp serial_lib.cpp)
target_include_directories(serial_lib PUBLIC "../3rdparty/")
set_target_properties(serial_lib PROPERTIES CXX_STANDARD 17)

add_executable(serial2 serial2.cpp)
target_link_libraries(serial2 serial_lib)