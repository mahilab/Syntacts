#include "Workspace.hpp"
#include "Gui.hpp"

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
    ImGui::BeginFixed(getName().c_str(), rect.getPosition(), rect.getSize());
    if (ImGui::BeginTabBar("WorkspaceTabs"))
    {
        if (ImGui::BeginTabItem("Designer##Tab"))
        {
            designer.render();
            ImGui::EndTabItem();
            gui->visualizer->setRenderedSignal(designer.buildSignal(), Purples::Plum);
        }
        if (ImGui::BeginTabItem("Sequencer##Tab"))
        {
            sequencer.render();
            ImGui::EndTabItem();
            gui->visualizer->setRenderedSignal(tact::Signal(), Oranges::Coral);
        }
        if (ImGui::BeginTabItem("Spatializer##Tab"))
        {
            spatializer.render();
            ImGui::EndTabItem();
            gui->visualizer->setRenderedSignal(spatializer.signal, Greens::YellowGreen);
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void Workspace::update()
{
    render();
}