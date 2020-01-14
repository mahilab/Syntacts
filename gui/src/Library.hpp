#pragma once

#include "Widget.hpp"
#include <filesystem>
#include "StatusBar.hpp"
#include "Designer.hpp"
#include <set>
#include "Palette.hpp"
#include "Visualizer.hpp"

namespace fs = std::filesystem;

class Library : public Widget, public carnot::GameObject {
public:

    Library(Gui* gui);

    struct Entry {
        std::string name;
        fs::path path;
    };

    const std::string& getSelectedName();
    tact::Signal getSelectedSignal();
private:
    void start();
    carnot::Enumerator periodicSync();
    void sync();
    void update() override;
    void render() override;
    void renderCreateDialog();
    void renderLibraryList();
    void renderLibraryControls();
    void onFileDrop(const std::string& filePath, const carnot::Vector2u& pos);
public:
    carnot::FloatRect rect;
    Palette palette;
private:
    std::string m_selected;
    int m_selection = -1;
    char m_inputBuffer[64] = "";
    std::map<std::string, Entry> m_lib;
};