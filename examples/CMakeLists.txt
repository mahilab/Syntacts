find_package(MEL REQUIRED)

add_executable(ex_headphone ex_headphone.cpp)
target_link_libraries(ex_headphone portaudio_static syntacts MEL::MEL)

add_executable(example example.cpp)
target_link_libraries(example portaudio_static syntacts MEL::MEL)