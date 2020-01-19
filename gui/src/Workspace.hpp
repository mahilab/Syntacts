#pragma once
#include "Designer.hpp"
#include "Sequencer.hpp"
#include "Spatializer.hpp"

class Workspace : public Widget, public carnot::GameObject {
public:
    enum Tab {
        TabDesigner,
        TabSequencer,
        TabSpatializer
    };
    Workspace(Gui* gui);
    void render() override;
    void update() override;
public:
    carnot::FloatRect rect;
public:
    Tab activeTab;
    Designer designer;
    Sequencer sequencer;
    Spatializer spatializer;   
};