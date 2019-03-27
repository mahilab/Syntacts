find_package(MEL REQUIRED)

add_executable(example example.cpp)
target_link_libraries(example tfx MEL::MEL)