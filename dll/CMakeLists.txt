add_library(syntacts-c SHARED "")
add_library(syntacts::syntacts-c ALIAS syntacts-c)
target_sources(syntacts-c PRIVATE "dll.hpp" "dll.cpp")
target_link_libraries(syntacts-c PRIVATE syntacts)

add_executable(test test.cpp)
target_link_libraries(test syntacts-c)