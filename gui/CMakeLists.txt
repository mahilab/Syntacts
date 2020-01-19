set(ICON syntacts.rc)
add_executable(syntacts-gui 
    src/main.cpp 
    src/Gui.hpp
    src/Gui.cpp
    src/Widget.hpp
    src/Widget.cpp
    src/DeviceBar.hpp
    src/DeviceBar.cpp
    src/Player.hpp
    src/Player.cpp
    src/StatusBar.hpp
    src/StatusBar.cpp
    src/Palette.hpp
    src/Palette.cpp
    src/Library.hpp
    src/Library.cpp
    src/Nodes.hpp
    src/Nodes.cpp
    src/Workspace.hpp
    src/Workspace.cpp
    src/Designer.hpp
    src/Designer.cpp
    src/Sequencer.hpp
    src/Sequencer.cpp
    src/Spatializer.hpp
    src/Spatializer.cpp
    src/Visualizer.hpp
    src/Visualizer.cpp
    src/DragAndDrop.hpp
    src/DragAndDrop.cpp
    src/ImGui/Sequencer.hpp
    src/ImGui/Sequencer.cpp
    src/ImGui/Custom.hpp
    src/ImGui/Custom.cpp
    src/ImGui/PolyBezier.hpp
    src/ImGui/PolyBezier.cpp
    src/ImGui/Theme.hpp
    src/ImGui/Theme.cpp
    ${ICON}
)
target_link_libraries(syntacts-gui syntacts carnot)
set_target_properties(syntacts-gui PROPERTIES DEBUG_POSTFIX -d)
install(TARGETS syntacts-gui RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})