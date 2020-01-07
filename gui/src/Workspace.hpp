#pragma once
#include "Designer.hpp"
#include "Sequencer.hpp"
#include "Spatializer.hpp"

class Workspace : public Widget, public carnot::GameObject {
public:
    Workspace(Gui* gui);
    void render() override;
    void update() override;
public:
    carnot::FloatRect rect;
public:
    Designer designer;
    Sequencer sequencer;
    Spatializer spatializer;   
};