add_library(syntacts-c SHARED "")
add_library(syntacts::syntacts-c ALIAS syntacts-c)
target_sources(syntacts-c PRIVATE "syntacts-c.hpp" "syntacts-c.cpp")
target_link_libraries(syntacts-c PRIVATE syntacts)
set_target_properties(syntacts-c PROPERTIES CXX_STANDARD 17 DEBUG_POSTFIX -d)
install(TARGETS syntacts-c RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

add_custom_command(TARGET syntacts-c POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntacts-c>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_basic/")

add_custom_command(TARGET syntacts-c POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntacts-c>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_library/")

add_custom_command(TARGET syntacts-c POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntacts-c>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_music/")

add_custom_command(TARGET syntacts-c POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntacts-c>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_sequences/")

add_custom_command(TARGET syntacts-c POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntacts-c>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_signals/")

add_custom_command(TARGET syntacts-c POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntacts-c>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_spatializer/")

               add_custom_command(TARGET syntacts-c POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntacts-c>              
               "${PROJECT_SOURCE_DIR}/csharp/examples/example_devices/")

add_custom_command(TARGET syntacts-c POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:syntacts-c>              
                "${PROJECT_SOURCE_DIR}/python/")

add_custom_command(TARGET syntacts-c POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:syntacts-c>              
                "${PROJECT_SOURCE_DIR}/unity/SyntactsDemo/Assets/Syntacts/Plugins/")

add_custom_command(TARGET syntacts-c POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${PROJECT_SOURCE_DIR}/csharp/Syntacts/Syntacts.cs"             
                "${PROJECT_SOURCE_DIR}/unity/SyntactsDemo/Assets/Syntacts/")

