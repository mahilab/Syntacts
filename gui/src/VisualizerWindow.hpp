#pragma once

#include <carnot>
#include <syntacts>
#include "helpers.hpp"


///////////////////////////////////////////////////////////////////////////////

using namespace carnot;

class VisualizerWindow : public GameObject {
public:

    using GameObject::GameObject;

    void setRenderedSignal(tact::Signal sig, Color color) {
        m_signal = std::move(sig);
        m_color  = color;   
    }

private:

    tact::Signal m_signal;
    Color m_color;

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
        m_bg = make<Shape>(4);
        m_bg->setPoint(0, rect.getPosition() + Vector2f(5,5));
        m_bg->setPoint(1, rect.getPosition() + Vector2f(rect.width, 0) + Vector2f(-5,5));
        m_bg->setPoint(2, rect.getPosition() + rect.getSize() + Vector2f(-5,-5));
        m_bg->setPoint(3, rect.getPosition() + Vector2f(0, rect.height) + Vector2f(5,-5));
        m_bgRend = addComponent<ShapeRenderer>(m_bg);
        m_bgRend->setColor(ImGui::GetStyle().Colors[ImGuiCol_Header]);
        m_plotStart = Vector2f(rect.left + 5, rect.top + 0.5f * rect.height);
        m_plotEnd   = Vector2f(rect.left + rect.width - 5, rect.top + 0.5f * rect.height);
        xDist = (m_plotEnd - m_plotStart).x / nPoints;
        yScale = -(rect.height - 10)/2 * 0.9f;
        m_plot = addComponent<LineRenderer>();
        m_plot->setPointCount(nPoints);


    }

    void update() override {
        helpers::setWindowRect(rect);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, Color::Transparent);
        ImGui::Begin(getName().c_str(), nullptr,  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar);
        render();    
        ImGui::End();    
        ImGui::PopStyleColor(); 
    }

    void render() {
        m_plot->setColor(m_color);
        float len = m_signal.length();
        if (len > 0) {
            m_plot->setEnabled(true);
            if (len == tact::INF) {
                ImGui::Text("INF");
                len = 0.1;
            }
            else
                ImGui::Text("%0.3f s", len);   
            float tDist = len / nPoints;
            float t = 0;
            float x = m_plotStart.x;

            for (int i = 0; i < nPoints; ++i) {     
                float s = Math::clamp<float>(m_signal.sample(t),-1,1);    
                float y = m_plotStart.y + s * yScale;
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

};