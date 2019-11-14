add_library(syntacts-plugin SHARED "")
add_library(syntacts::syntacts-plugin ALIAS syntacts-plugin)
target_sources(syntacts-plugin PRIVATE "plugin.hpp" "plugin.cpp")
target_link_libraries(syntacts-plugin PRIVATE syntacts)