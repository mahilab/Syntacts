#pragma once

#include <carnot>
#include <syntacts>
#include "gui-detail.hpp"
#include "DesignerWindow.hpp"
#include "LibraryWindow.hpp"

using namespace carnot;

class VisualizerWindow : public GameObject {
public:

    using GameObject::GameObject;

private:

    void start() override {
        m_designer = findSibling<DesignerWindow>();
        m_library  = findSibling<LibraryWindow>();
    }

    void update() override {
        helpers::setWindowRect(rect);
        ImGui::Begin(getName().c_str(), nullptr,  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar);
        updatePlot();
        ImGui::End();        
    }

    /// Updates the waveform plot
    void updatePlot() {
        
        // auto des = m_designer->buildCue();
        // auto lib = m_library->getSelectedCue();

        // auto nPoints = std::max(des.length() * 48000, lib.length() * 48000);

        // m_desPlot.resize(nPoints); 
        // m_libPlot.resize(nPoints);

        // float t = 0;
        // for (int i = 0; i < nPoints; ++i) {
        //     m_desPlot[i] = des.sample(t) + 1;
        //     m_libPlot[i] = lib.sample(t) - 1;
        //     t += 0.0001f;
        // }   

        
        // float avail = ImGui::GetContentRegionAvailWidth()+1;
        // m_width = Math::clamp(m_width, avail, 10000.0f);
        // ImGui::PushItemWidth(m_width);
        // ImGui::PushStyleColor(ImGuiCol_PlotLines, hexCode("cf94c2"));
        // ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, hexCode("cf94c2"));
        // ImGui::PlotLines2("", &m_desPlot[0], &m_libPlot[0], nPoints,  0, "", -2.0f, 2.0f, ImVec2(0, ImGui::GetContentRegionAvail().y));

        // if (ImGui::IsItemHovered()) {
        //     m_width -= Input::getScroll() * m_width / 20.0f; 
        //     if (Input::getDoubleClick(MouseButton::Left))
        //         m_width = avail;
        // }
        // ImGui::PopStyleColor();
        // ImGui::PopStyleColor();
        // ImGui::PopItemWidth();
    }

public:

    FloatRect rect;

private:

    std::vector<float> m_desPlot;
    std::vector<float> m_libPlot;
    Handle<DesignerWindow> m_designer;
    Handle<LibraryWindow>  m_library;

    float m_width = 0;

};