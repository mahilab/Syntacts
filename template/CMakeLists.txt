cmake_minimum_required(VERSION 3.13.0)
project(MySyntactsProject VERSION 1.0.0)

find_package(syntacts REQUIRED)

add_executable(myApp main.cpp)
target_link_libraries(myApp syntacts)