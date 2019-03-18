add_executable(example example.cpp)
target_link_libraries(example tfx portaudio_static MEL::MEL)

add_executable(sawmono saw_mono.cpp)
target_link_libraries(sawmono tfx portaudio_static MEL::MEL)