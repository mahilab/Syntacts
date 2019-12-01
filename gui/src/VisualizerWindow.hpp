#pragma once

#include <carnot>
#include <syntacts>
#include "DesignerWindow.hpp"
#include "LibraryWindow.hpp"

using namespace carnot;

class VisualizerWindow : public GameObject {
public:

    using GameObject::GameObject;

private:

    Vector2f m_plotStart;
    Vector2f m_plotEnd;
    Handle<ShapeRenderer> m_bgRend;
    Ptr<Shape> m_bg;
    Handle<LineRenderer> m_plot;
    float xDist;
    float yScale;
    int nPoints = 10000;

    void start() override {
        m_designer = findSibling<DesignerWindow>();
        m_library  = findSibling<LibraryWindow>();

        m_bg = make<Shape>(4);
        m_bg->setPoint(0, rect.getPosition() + Vector2f(5,5));
        m_bg->setPoint(1, rect.getPosition() + Vector2f(rect.width, 0) + Vector2f(-5,5));
        m_bg->setPoint(2, rect.getPosition() + rect.getSize() + Vector2f(-5,-5));
        m_bg->setPoint(3, rect.getPosition() + Vector2f(0, rect.height) + Vector2f(5,-5));
        // m_bg->setRadii(2);
        m_bgRend = addComponent<ShapeRenderer>(m_bg);
        m_bgRend->setColor(ImGui::GetStyle().Colors[ImGuiCol_Header]);

        m_plotStart = Vector2f(rect.left + 5, rect.top + 0.5f * rect.height);
        m_plotEnd   = Vector2f(rect.left + rect.width - 5, rect.top + 0.5f * rect.height);
        xDist = (m_plotEnd - m_plotStart).x / nPoints;

        yScale = -(rect.height - 10)/2 * 0.9f;

        m_plot = addComponent<LineRenderer>();
        m_plot->setColor(hexCode("cf94c2"));
        m_plot->setPointCount(nPoints);
    }

    void update() override {
        updatePlot2();
        helpers::setWindowRect(rect);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, Color::Transparent);
        ImGui::Begin(getName().c_str(), nullptr,  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::End();    
        ImGui::PopStyleColor(); 
    }

    void updatePlot2() {
        auto sig = m_designer->buildSignal();
        auto len = sig.length();
        if (len > 0) {
            m_plot->setEnabled(true);
            if (len == tact::INF)
                len = 0.1f;
            float tDist = len / nPoints;
            float t = 0;
            float x = m_plotStart.x;
            for (int i = 0; i < nPoints; ++i) {                
                float y = m_plotStart.y + sig.sample(t) * yScale;
                m_plot->setPoint(i, x, y);
                t += tDist;
                x += xDist;
            }
        }
        else {
            m_plot->setEnabled(false);
        }
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