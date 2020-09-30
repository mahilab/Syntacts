add_library(syntacts_c SHARED "")
add_library(syntacts::syntacts_c ALIAS syntacts_c)
target_sources(syntacts_c PRIVATE "syntacts.h" "syntacts.cpp")
target_link_libraries(syntacts_c PRIVATE syntacts)
set_target_properties(syntacts_c PROPERTIES CXX_STANDARD 17 DEBUG_POSTFIX -d)
install(TARGETS syntacts_c RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

add_custom_command(TARGET syntacts_c POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntacts_c>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_basic/")

add_custom_command(TARGET syntacts_c POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntacts_c>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_library/")

add_custom_command(TARGET syntacts_c POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntacts_c>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_music/")

add_custom_command(TARGET syntacts_c POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntacts_c>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_sequences/")

add_custom_command(TARGET syntacts_c POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntacts_c>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_signals/")

add_custom_command(TARGET syntacts_c POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntacts_c>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_spatializer/")

add_custom_command(TARGET syntacts_c POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntacts_c>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_devices/")

add_custom_command(TARGET syntacts_c POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:syntacts_c>              
                "${PROJECT_SOURCE_DIR}/python/")

add_custom_command(TARGET syntacts_c POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:syntacts_c>              
                "${PROJECT_SOURCE_DIR}/unity/SyntactsDemo/Assets/Syntacts/Plugins/")

add_custom_command(TARGET syntacts_c POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${PROJECT_SOURCE_DIR}/csharp/Syntacts/Syntacts.cs"             
                "${PROJECT_SOURCE_DIR}/unity/SyntactsDemo/Assets/Syntacts/")

