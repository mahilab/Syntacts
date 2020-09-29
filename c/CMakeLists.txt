add_library(syntactsc SHARED "")
add_library(syntacts::syntactsc ALIAS syntactsc)
target_sources(syntactsc PRIVATE "syntacts.h" "syntacts.cpp")
target_link_libraries(syntactsc PRIVATE syntacts)
set_target_properties(syntactsc PROPERTIES CXX_STANDARD 17 DEBUG_POSTFIX -d)
install(TARGETS syntactsc RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

add_custom_command(TARGET syntactsc POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntactsc>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_basic/")

add_custom_command(TARGET syntactsc POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntactsc>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_library/")

add_custom_command(TARGET syntactsc POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntactsc>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_music/")

add_custom_command(TARGET syntactsc POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntactsc>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_sequences/")

add_custom_command(TARGET syntactsc POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntactsc>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_signals/")

add_custom_command(TARGET syntactsc POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntactsc>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_spatializer/")

add_custom_command(TARGET syntactsc POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntactsc>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_devices/")

add_custom_command(TARGET syntactsc POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:syntactsc>              
                "${PROJECT_SOURCE_DIR}/python/")

add_custom_command(TARGET syntactsc POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:syntactsc>              
                "${PROJECT_SOURCE_DIR}/unity/SyntactsDemo/Assets/Syntacts/Plugins/")

add_custom_command(TARGET syntactsc POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${PROJECT_SOURCE_DIR}/csharp/Syntacts/Syntacts.cs"             
                "${PROJECT_SOURCE_DIR}/unity/SyntactsDemo/Assets/Syntacts/")

