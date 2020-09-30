add_executable(dll dll.cpp)
target_link_libraries(dll syntacts_c)
target_include_directories(dll PUBLIC "../c/")

add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark syntacts)