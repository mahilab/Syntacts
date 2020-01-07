#include "Player.hpp"
#include "Gui.hpp"
#include "ImGui/Custom.hpp"

using namespace carnot;

void Player::start()
{
    gui->device->onSessionOpen.connect(this, &Player::rechannel);
    rechannel();
}

void Player::update() {
    render();
}

void Player::render()
{
    ImGui::BeginFixed(getName().c_str(), rect.getPosition(), rect.getSize());
    ImGui::BeginNodeTarget();
    if (ImGui::Button(ICON_FA_PLAY, ImVec2(25, 0)))
        playSelected();
    ImGui::EndNodeTarget();
    if (ImGui::NodeDroppedL()) {
        tact::Signal sig;
        if (tact::Library::loadSignal(sig, ImGui::NodePayloadL()))
        {
            for (int i = 0; i < gui->device->session->getCurrentDevice().maxChannels; ++i)
                gui->device->session->play(i,sig);
        }
    }
    if (ImGui::IsItemClicked(1)) {
        for (int i = 0; i < gui->device->session->getCurrentDevice().maxChannels; ++i)
            gui->device->session->stop(i);
    }
    ImGui::SameLine();
    ImGui::PushItemWidth(-1);
    if (ImGui::SliderFloat("##Master", &m_masterVol, 0, 1, ""))
    {
        for (int i = 0; i < gui->device->session->getCurrentDevice().maxChannels; ++i)
        {
            m_channelVol[i] = m_masterVol;
            gui->device->session->setVolume(i, m_masterVol);
        }
    }
    ImGui::PopItemWidth();
    ImGui::Separator();
    renderChannels();
    ImGui::End();
}

/// Updates the channel selection checkboxes
void Player::renderChannels()
{
    ImGui::BeginChild("Channels", ImVec2(0, 0), false, ImGuiWindowFlags_NoBackground);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y * 2));
    if (gui->device->session) {
        for (int i = 0; i < gui->device->session->getCurrentDevice().maxChannels; ++i)
        {
            auto label = str(i);
            ImGui::BeginNodeTarget();
            if (ImGui::Button(label.c_str(), ImVec2(25, 0)))
                playCh(i);
            ImGui::EndNodeTarget();
            if (ImGui::NodeDroppedL()) {
                tact::Signal sig;
                tact::Library::loadSignal(sig, ImGui::NodePayloadL());
                gui->device->session->play(i, sig);
            }
            if (ImGui::IsItemClicked(1))
                gui->device->session->stop(i);
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                m_payload = i;
                ImGui::SetDragDropPayload("DND_CHANNEL", &m_payload, sizeof(int));
                ImGui::Text(label.c_str());
                ImGui::EndDragDropSource();
            }

            ImGui::PushItemWidth(-1);

            ImGui::SameLine(30);
            if (ImGui::MiniSliderFloat(str("##Volume", i).c_str(), &m_channelVol[i], 0, 1, true))
                gui->device->session->setVolume(i, m_channelVol[i]);
            if (ImGui::IsItemClicked(1))
            {
                m_channelVol[i] = m_channelVol[i] == 0.0f ? 1.0f : m_channelVol[i] == 1.0f ? 0.0f : m_channelVol[i] < 0.5f ? 0.0f : 1.0f;
                gui->device->session->setVolume(i, m_channelVol[i]);
            }

            ImGui::SameLine(30);
            if (ImGui::MiniSliderFloat(str("##Pitch", i).c_str(), &m_channelPitch[i], -1, 1, false))
                gui->device->session->setPitch(i, (float)std::pow(10, m_channelPitch[i]));
            if (ImGui::IsItemClicked(1))
            {
                m_channelPitch[i] = 0;
                gui->device->session->setPitch(i, (float)std::pow(10, m_channelPitch[i]));
            }

            ImGui::PopItemWidth();
        }
    }
    ImGui::PopStyleVar();
    ImGui::EndChild();
}

void Player::playCh(int ch)
{
    auto sig = gui->workspace->designer.buildSignal();
    gui->device->session->play(ch, sig);
}

/// Plays all selected channels
void Player::playSelected()
{
    for (int i = 0; i < gui->device->session->getCurrentDevice().maxChannels; ++i)
        playCh(i);
}

/// Gets number of channels from current Syntacts device and resizes accordingly
void Player::rechannel()
{
    int maxChannels = gui->device->session ? gui->device->session->getCurrentDevice().maxChannels : 0;
    m_channelVol = std::vector<float>(maxChannels, 1.0f);
    m_channelPitch = std::vector<float>(maxChannels, 0.0f);
}