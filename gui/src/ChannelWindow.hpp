#pragma once

#include <carnot>
#include "DeviceBar.hpp"
#include "DesignerWindow.hpp"
#include "LibraryWindow.hpp"
#include <Syntacts/Syntacts.hpp>
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
        m_deviceBar->onInitialize.connect(this, &ChannelWindow::rechannel);
        m_deviceBar->onSwitchDevice.connect(this, &ChannelWindow::rechannel);
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
        for (int i = 0; i < m_deviceBar->session->getCurrentDevice().maxChannels; ++i)
        {
            auto label = str(i + 1);
            if (ImGui::Button(label.c_str(), ImVec2(25,0))) // || Input::getKeyDown((Key)((int)Key::Num1 + i)))
                playCh(i);
            ImGui::SameLine();
            // ImGui::Checkbox(str("##", i).c_str(), &m_checkBoxes[i]);
            ImGui::PushItemWidth(-1);
            if (ImGui::SliderFloat(str("##", i).c_str(), &m_channelVol[i], 0, 1, "")) {
                m_deviceBar->session->setVolume(i, Math::pow(m_channelVol[i],4));
            }
            // if (ImGui::IsItemHovered() || ImGui::IsItemActive())
            //     ImGui::SetTooltip("%.3f", m_channelVol[i]);
            if (ImGui::IsItemClicked(1)) {
                m_channelVol[i] = m_channelVol[i] == 0.0f ? 1.0f : m_channelVol[i] == 1.0f ? 0.0f : m_channelVol[i] < 0.5f ? 0.0f : 1.0f;
                m_deviceBar->session->setVolume(i, Math::pow(m_channelVol[i],1));
            }
            // ImGui::SameLine();
        }
        ImGui::EndChild();
        // if (ImGui::Button("Select All"))
        //     selectAllChannels(true);
        // ImGui::SameLine();
        // if(ImGui::Button("Deselect All"))
        //     selectAllChannels(false);
        ImGui::PopStyleColor(); 

    }

    /// Creats and plays the user's cue
    void playCh(int ch) {
        auto cue =  m_designer->buildCue();
        // auto cue = m_library->getSelectedCue();
        if (cue) {
            m_deviceBar->session->play(ch, cue);
        }
    }

    /// Plays all selected channels
    void playSelected() {
        for (int i = 0; i < m_deviceBar->session->getCurrentDevice().maxChannels; ++i) {
            if (m_channelVol[i] > 0)
                playCh(i);
        }
    }

    /// Gets number of channels from current Syntacts device and resizes accordingly
    void rechannel() {
        m_channelVol = std::vector<float>(m_deviceBar->session->getCurrentDevice().maxChannels, 1.0f);
    }

public:

    FloatRect rect;

private:

    float              m_masterVol = 1.0f;
    std::vector<float> m_channelVol;

};