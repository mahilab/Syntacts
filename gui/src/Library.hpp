#pragma once

#include "Widget.hpp"
#include "StatusBar.hpp"
#include "Designer.hpp"
#include <set>
#include "Palette.hpp"
#include "Visualizer.hpp"
#include "FileWatcher.hpp"


class Library : public Widget {
public:

    Library(Gui& gui);

    struct Entry {
        std::string name;
        tact::Signal disk;
        bool loaded = false;
    };

    void update() override;
private:
    void init();
    void renderCreateDialog();
    void renderLibraryList();
    void renderLibraryControls();    
    tact::Signal getSelectedSignal();
private:

    void onFileChange(std::string path, FileStatus status);
    void onFileDrop(const std::vector<std::string>& paths);

public:
    Palette palette;
private:
    std::mutex m_mtx;
    FileWatcher m_watcher;
    std::string m_selected;
    int m_selection = -1;
    char m_inputBuffer[64] = "";
    std::map<std::string, Entry> m_lib;
    bool m_ignoreFilChange = false;
    bool m_focused;
};