#pragma once
#include "Designer.hpp"
#include "Sequencer.hpp"
#include "Spatializer.hpp"

class Workspace : public Widget {
public:
    enum Tab {
        TabDesigner,
        TabSequencer,
        TabSpatializer
    };
    Workspace(Gui& gui);
    void update() override;
public:
    Tab activeTab;
    Designer designer;
    Sequencer sequencer;
    Spatializer spatializer;   
};