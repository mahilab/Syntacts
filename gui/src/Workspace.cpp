#include "Workspace.hpp"
#include "Gui.hpp"
#include "ImGui/Sequencer.hpp"

using namespace carnot;

Workspace::Workspace(Gui *gui) : Widget(gui),
                                 designer(gui),
                                 sequencer(gui),
                                 spatializer(gui)
{
}

void Workspace::render()
{
    ImGui::BeginFixed("Workspace", rect.getPosition(), rect.getSize());
    if (ImGui::BeginTabBar("WorkspaceTabs"))
    {
        if (ImGui::BeginTabItem("Designer##Tab"))
        {
            activeTab = TabDesigner;
            designer.render();
            ImGui::EndTabItem();
            gui->visualizer->setRenderedSignal(designer.buildSignal(), Purples::Plum);
        }
        if (ImGui::BeginTabItem("Sequencer##Tab"))
        {
            activeTab = TabSequencer;
            sequencer.render();
            ImGui::EndTabItem();
            gui->visualizer->setRenderedSignal(sequencer.buildSignal(), Oranges::Coral);
        }
        if (ImGui::BeginTabItem("Spatializer##Tab"))
        {
            activeTab = TabSpatializer;
            spatializer.render();
            ImGui::EndTabItem();
            gui->visualizer->setRenderedSignal(spatializer.getSignal(), Greens::YellowGreen);
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void Workspace::update()
{
    render();
}