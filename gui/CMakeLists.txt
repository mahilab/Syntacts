find_package(carnot REQUIRED)

set(ICON tfx.rc)

add_executable(tfx-gui tfx-gui.cpp ${ICON})
target_link_libraries(tfx-gui tfx carnot)

add_executable(tfx-gui_2ch tfx-gui_2ch.cpp ${ICON})
target_link_libraries(tfx-gui_2ch tfx carnot)

add_executable(lockless lockless.cpp)
target_link_libraries(lockless tfx carnot)