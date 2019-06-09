find_package(carnot REQUIRED)

set(ICON syntacts.rc)

add_executable(syntacts-gui gui.cpp ${ICON})
target_link_libraries(syntacts-gui syntacts carnot)