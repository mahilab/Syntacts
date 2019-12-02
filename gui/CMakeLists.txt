set(ICON syntacts.rc)
add_executable(syntacts-gui 
    src/main.cpp 
    src/ChannelWindow.hpp
    src/DesignerWindow.hpp
    src/DeviceBar.hpp
    src/LibraryWindow.hpp
    src/VisualizerWindow.hpp
    src/SignalNodes.hpp
    src/InfoBar.hpp
    src/helpers.hpp
    src/ImGui/ImSequencer.hpp
    src/ImGui/ImSequencer.cpp
    ${ICON}
)
target_link_libraries(syntacts-gui syntacts carnot)
set_target_properties(syntacts-gui PROPERTIES DEBUG_POSTFIX -d)
install(TARGETS syntacts-gui RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})