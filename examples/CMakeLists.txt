add_executable(example example.cpp)
target_link_libraries(example tfx portaudio_static MEL::MEL)
target_compile_definitions(example PRIVATE TFX_STATIC)