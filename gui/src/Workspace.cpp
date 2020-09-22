#include "Workspace.hpp"
#include "Gui.hpp"

using namespace mahi::gui;

Workspace::Workspace(Gui& gui) : Widget(gui),
                                 designer(gui),
                                 sequencer(gui),
                                 spatializer(gui)
{
}

void Workspace::update()
{
    ImGui::BeginFixed("Workspace", position, size, ImGuiWindowFlags_NoTitleBar);
    if (ImGui::BeginTabBar("WorkspaceTabs"))
    {
        if (ImGui::BeginTabItem(" Designer ##Tab"))
        {
            activeTab = TabDesigner;
            designer.update();
            ImGui::EndTabItem();
            gui.visualizer.setRenderedSignal(designer.buildSignal(), gui.theme.designerColor);
        }
        if (ImGui::BeginTabItem(" Sequencer ##Tab"))
        {
            activeTab = TabSequencer;
            sequencer.update();
            ImGui::EndTabItem();
            gui.visualizer.setRenderedSignal(sequencer.buildSignal(), gui.theme.sequencerColor);
        }
        if (ImGui::BeginTabItem(" Spatializer ##Tab"))
        {
            activeTab = TabSpatializer;
            spatializer.update();
            ImGui::EndTabItem();
            gui.visualizer.setRenderedSignal(spatializer.getSignal(), gui.theme.spatializerColor);
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}