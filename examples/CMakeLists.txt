macro(syntacts_example SYNTACTS_EXAMPLE_NAME)
    add_executable(${SYNTACTS_EXAMPLE_NAME} "example_${SYNTACTS_EXAMPLE_NAME}.cpp")
    target_link_libraries(${SYNTACTS_EXAMPLE_NAME} syntacts)
    set_target_properties(${SYNTACTS_EXAMPLE_NAME} PROPERTIES DEBUG_POSTFIX -d)
    install(TARGETS ${SYNTACTS_EXAMPLE_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endmacro()

syntacts_example(basic)
syntacts_example(signals)
syntacts_example(devices)
syntacts_example(library)
syntacts_example(music)
syntacts_example(rtti)
syntacts_example(sequences)
syntacts_example(spatializer)