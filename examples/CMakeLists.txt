macro(syntacts_example SYNTACTS_EXAMPLE_NAME)
    add_executable(${SYNTACTS_EXAMPLE_NAME} "example_${SYNTACTS_EXAMPLE_NAME}.cpp")
    target_link_libraries(${SYNTACTS_EXAMPLE_NAME} syntacts)
    set_target_properties(${SYNTACTS_EXAMPLE_NAME} PROPERTIES DEBUG_POSTFIX -d)
    install(TARGETS ${SYNTACTS_EXAMPLE_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endmacro()


# find_package(MEL REQUIRED)

# add_executable(ex_headphone ex_headphone.cpp)
# target_link_libraries(ex_headphone portaudio_static syntacts MEL::MEL)

# add_executable(example example.cpp)
# target_link_libraries(example portaudio_static syntacts MEL::MEL)