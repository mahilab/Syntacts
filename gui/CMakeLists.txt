find_package(carnot REQUIRED)

set(ICON syntacts.rc)

add_executable(syntacts-gui gui-main.cpp gui-detail.hpp ${ICON})
target_link_libraries(syntacts-gui syntacts carnot)