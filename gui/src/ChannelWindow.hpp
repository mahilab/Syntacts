#pragma once

#include "ImGui/CustomWidgets.hpp"
#include <carnot>
#include <syntacts>
#include "DeviceBar.hpp"
#include "DesignerWindow.hpp"
#include "LibraryWindow.hpp"
#include "helpers.hpp"
#include <deque>

using namespace carnot;

class ChannelWindow : public GameObject {
public:

    using GameObject::GameObject;

private:

    Handle<DeviceBar> m_deviceBar;
    Handle<DesignerWindow> m_designer;
    Handle<LibraryWindow> m_library;

    void start() {
        m_deviceBar = findSibling<DeviceBar>();
        m_designer = findSibling<DesignerWindow>();
        m_library = findSibling<LibraryWindow>();
        m_deviceBar->onDeviceUpdated.connect(this, &ChannelWindow::rechannel);
        rechannel();
    }

    void update() override {
        helpers::setWindowRect(rect);
        ImGui::Begin(getName().c_str(), nullptr,  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        if (ImGui::Button(ICON_FA_PLAY, ImVec2(25, 0))) 
            playSelected();

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);
        if (ImGui::SliderFloat("##Master", &m_masterVol, 0, 1, "")) {
            for (int i = 0; i < m_deviceBar->session->getCurrentDevice().maxChannels; ++i) {
                m_channelVol[i] = m_masterVol;
                m_deviceBar->session->setVolume(i, m_masterVol);
            }
        }
        ImGui::PopItemWidth();
        ImGui::Separator();
        updateChannels();
        ImGui::End();
    }

    /// Updates the channel selection checkboxes
    void updateChannels() {
        ImGui::PushStyleColor(ImGuiCol_Border, Color::Transparent);
        ImGui::BeginChild("Channels", ImVec2(0,0), false, ImGuiWindowFlags_NoBackground);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2( ImGui::GetStyle().FramePadding.x,  ImGui::GetStyle().FramePadding.y * 2 ));

        for (int i = 0; i < m_deviceBar->session->getCurrentDevice().maxChannels; ++i)
        {
            auto label = str(i + 1);


            if (ImGui::Button(label.c_str(), ImVec2(25,0)))
                playCh(i);
            if (ImGui::IsItemClicked(1)) 
                m_deviceBar->session->stop(i);

            ImGui::PushItemWidth(-1);

            ImGui::SameLine(30);
            if (ImGui::MiniSliderFloat(str("##Volume", i).c_str(), &m_channelVol[i], 0, 1, true))
                m_deviceBar->session->setVolume(i, m_channelVol[i]);
            if (ImGui::IsItemClicked(1)) {
                m_channelVol[i] = m_channelVol[i] == 0.0f ? 1.0f : m_channelVol[i] == 1.0f ? 0.0f : m_channelVol[i] < 0.5f ? 0.0f : 1.0f;
                m_deviceBar->session->setVolume(i, m_channelVol[i]);
            }

            ImGui::SameLine(30);
            if (ImGui::MiniSliderFloat(str("##Pitch", i).c_str(), &m_channelPitch[i], -1, 1, false))
                m_deviceBar->session->setPitch(i, std::pow(10,m_channelPitch[i]));
            if (ImGui::IsItemClicked(1)) {
                m_channelPitch[i] = 0;
                m_deviceBar->session->setPitch(i, m_channelPitch[i]);
            }

            ImGui::PopItemWidth();

        }
        ImGui::PopStyleVar();

        ImGui::EndChild();
        ImGui::PopStyleColor(); 

    }

    /// Creates and plays the user's cue
    void playCh(int ch) {
        auto cue =  m_designer->buildSignal();
        m_deviceBar->session->play(ch, cue);
    }

    /// Plays all selected channels
    void playSelected() {
        for (int i = 0; i < m_deviceBar->session->getCurrentDevice().maxChannels; ++i)
            playCh(i);
    }

    /// Gets number of channels from current Syntacts device and resizes accordingly
    void rechannel() {
        m_channelVol   = std::vector<float>(m_deviceBar->session->getCurrentDevice().maxChannels, 1.0f);
        m_channelPitch = std::vector<float>(m_deviceBar->session->getCurrentDevice().maxChannels, 0.0f);
    }

public:

    FloatRect rect;

private:

    float              m_masterVol = 1.0f;
    std::vector<float> m_channelVol;
    std::vector<float> m_channelPitch;

};