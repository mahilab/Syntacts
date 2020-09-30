add_executable(syntacts_gui
    src/main.cpp 
    src/Gui.hpp
    src/Gui.cpp
    src/Widget.hpp
    src/Widget.cpp
    src/DeviceBar.hpp
    src/DeviceBar.cpp
    src/StatusBar.hpp
    src/Palette.hpp
    src/Palette.cpp
    src/StatusBar.cpp
    src/Visualizer.hpp
    src/Visualizer.cpp
    src/Library.hpp
    src/Library.cpp
    src/FileWatcher.hpp
    src/FileWatcher.cpp
    src/Nodes.hpp
    src/Nodes.cpp
    src/DragAndDrop.hpp
    src/DragAndDrop.cpp
    src/Workspace.hpp
    src/Workspace.cpp
    src/Designer.hpp
    src/Designer.cpp
    src/PolyBezier.hpp
    src/PolyBezier.cpp
    src/Sequencer.hpp
    src/Sequencer.cpp
    src/Spatializer.hpp
    src/Spatializer.cpp
    src/Player.hpp
    src/Player.cpp
    src/Debugger.hpp
    src/Debugger.cpp
    src/Theme.hpp
    src/Theme.cpp
    src/Custom.hpp
    src/Custom.cpp
    syntacts.rc)

target_link_libraries(syntacts_gui syntacts mahi::gui)
set_target_properties(syntacts_gui PROPERTIES DEBUG_POSTFIX -d)
install(TARGETS syntacts_gui RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})