add_library(syntacts-c SHARED "")
add_library(syntacts::syntacts-c ALIAS syntacts-c)
target_sources(syntacts-c PRIVATE "dll.hpp" "dll.cpp")
target_link_libraries(syntacts-c PRIVATE syntacts)
set_target_properties(syntacts-c PROPERTIES CXX_STANDARD 17 DEBUG_POSTFIX -d)

add_custom_command(TARGET syntacts-c POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               $<TARGET_FILE:syntacts-c>              
               "${PROJECT_SOURCE_DIR}/csharp/")

add_custom_command(TARGET syntacts-c POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:syntacts-c>              
                "${PROJECT_SOURCE_DIR}/python/")

add_custom_command(TARGET syntacts-c POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:syntacts-c>              
                "${PROJECT_SOURCE_DIR}/unity/SyntactsDemo/Assets/Plugins/")

add_custom_command(TARGET syntacts-c POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${PROJECT_SOURCE_DIR}/csharp/Syntacts.cs"             
                "${PROJECT_SOURCE_DIR}/unity/SyntactsDemo/Assets/")

add_executable(test test.cpp)
target_link_libraries(test syntacts-c)

