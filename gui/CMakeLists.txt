# set(SFML_STATIC_LIBRARIES TRUE)
# find_package(SFML 2.5 REQUIRED COMPONENTS audio graphics window system network)
# # find_package(Chipmunk REQUIRED)
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /await") # COROUTINES

# set(ICON tfx.rc)

# add_executable(tfx-gui tfx-gui.cpp ${ICON})
# target_include_directories(tfx-gui PUBLIC "C:/Git/carnot/include" "C:/Program Files/chipmunk/include")
# target_link_directories(tfx-gui PUBLIC "C:/Git/carnot/build-msvc-x64-release" "C:/Program Files/chipmunk/lib")
# target_link_libraries(
#     tfx-gui tfx portaudio_static carnot
#     ws2_32
#     winmm
#     Pdh
#     Psapi
#     XInput
#     Shcore
#     sfml-audio sfml-graphics sfml-window sfml-system sfml-network
#     chipmunk
#     )