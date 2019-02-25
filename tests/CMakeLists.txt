# executables
add_executable(evan_testing tests/evan_testing.cpp)
target_link_libraries(evan_testing portaudio_static MEL::MEL)

add_executable(devs tests/devs.cpp)
target_link_libraries(devs portaudio_static)

add_executable(saw_mono tests/pa_saw_mono.cpp)
target_link_libraries(saw_mono portaudio_static)

add_executable(six_channel_testing_2 tests/six_channel_testing_2.cpp)
target_link_libraries(six_channel_testing_2 portaudio_static MEL::MEL)

add_executable(two_channel_testing tests/two_channel_testing.cpp)
target_link_libraries(two_channel_testing portaudio_static MEL::MEL)

add_executable(two_channel_testing_time tests/two_channel_testing_time.cpp)
target_link_libraries(two_channel_testing_time portaudio_static MEL::MEL)

add_executable(sine_cxx tests/paex_sine_c++.cpp)
target_link_libraries(sine_cxx portaudio_static MEL::MEL)

add_executable(saw_six_ch tests/saw_six_ch.cpp)
target_link_libraries(saw_six_ch portaudio_static MEL::MEL)

add_executable(sine_six tests/sine_six.cpp)
target_link_libraries(sine_six portaudio_static MEL::MEL)

add_executable(sine_six_ASR tests/sine_six_ASR.cpp)
target_link_libraries(sine_six_ASR portaudio_static MEL::MEL)

add_executable(saw_six_ASR tests/saw_six_ASR.cpp)
target_link_libraries(saw_six_ASR portaudio_static MEL::MEL)

add_executable(christiansen tests/Christiansen_six.cpp)
target_link_libraries(christiansen portaudio_static MEL::MEL)

add_executable(double_sin_six tests/double_sin_six.cpp)
target_link_libraries(double_sin_six portaudio_static MEL::MEL)

add_executable(cue_test_PA tests/cue_test_PA.cpp)
target_link_libraries(cue_test_PA MEL::MEL portaudio_static)

add_executable(cue_test_AS tests/cue_test_AS.cpp)
target_link_libraries(cue_test_AS MEL::MEL)