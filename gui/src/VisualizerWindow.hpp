#pragma once

#include <carnot>
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
        m_cuePlot.clear();
        auto cue = m_designer->buildCue();
        auto nPoints = cue->sampleCount(10000);
        m_cuePlot.resize(nPoints);    
        float t = 0;
        for (auto& point : m_cuePlot) {
            point = cue->sample(t) + 1;
            t += 0.0001f;
        }
        m_envPlot.resize(nPoints);
        // auto env = m_designer->buildEnv();
        auto env = m_library->getSelectedCue();
        t = 0;
        for (auto& point : m_envPlot) {
            if (env)
                point = env->sample(t) - 1;
            else
                point = -3;
            t += 0.0001f;
        }
        
        float avail = ImGui::GetContentRegionAvailWidth()+1;
        m_width = Math::clamp(m_width, avail, 10000.0f);
        ImGui::PushItemWidth(m_width);
        ImGui::PushStyleColor(ImGuiCol_PlotLines, hexCode("cf94c2"));
        ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, hexCode("cf94c2"));
        auto title = str(cue->getEnvelope()->getDuration()*1000, "ms /", m_cuePlot.size(), " samples");
        ImGui::PlotLines2("", &m_cuePlot[0], &m_envPlot[0], nPoints,  0, "", -2.0f, 2.0f, ImVec2(0, ImGui::GetContentRegionAvail().y));
        if (ImGui::IsItemHovered()) {
            m_width -= Input::getScroll() * m_width / 20.0f; 
            if (Input::getDoubleClick(MouseButton::Left))
                m_width = avail;
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopItemWidth();
    }

public:

    FloatRect rect;

private:

    std::vector<float> m_cuePlot;
    std::vector<float> m_envPlot;
    Handle<DesignerWindow> m_designer;
    Handle<LibraryWindow>  m_library;

    float m_width = 0;

};