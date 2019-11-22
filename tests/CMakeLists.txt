# add_executable(benchmark benchmark.cpp)
# target_link_libraries(benchmark syntacts)

# # add_executable(type_erasure type_erasure.cpp)
# # target_link_libraries(type_erasure syntacts)

# add_executable(clone clone.cpp)
# target_link_libraries(clone syntacts)

add_executable(exp_api exp_api.cpp)
target_include_directories(exp_api PRIVATE "../3rdparty")

add_executable(json json.cpp)
# target_link_libraries(j)

add_executable(clone clone.cpp)
add_executable(clone2 clone2.cpp)

add_executable(te te.cpp)