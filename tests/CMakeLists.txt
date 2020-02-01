add_executable(dll dll.cpp)
target_link_libraries(dll syntacts-c)
target_include_directories(dll PUBLIC "../dll/")

add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark syntacts)