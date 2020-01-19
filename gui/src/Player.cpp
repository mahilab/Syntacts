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
    BeginNodeTarget();
    if (ImGui::Button(ICON_FA_PLAY, ImVec2(25, 0)))
        playSelected();
    EndNodeTarget();
    if (NodeDroppedL()) {
        tact::Signal sig;
        if (tact::Library::loadSignal(sig, NodePayloadL().first))
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
            ImGui::PushID(i);
            bool playing = gui->device->session->isPlaying(i);
            if (playing)
                ImGui::PushStyleColor(ImGuiCol_Button, Grays::Gray50);
            auto label = str(i);
            BeginNodeTarget();
            if (ImGui::Button(label.c_str(), ImVec2(25, 0)))
                playCh(i);
            EndNodeTarget();
            if (playing)
                ImGui::PopStyleColor();

            if (NodeDroppedL()) {
                tact::Signal sig;
                tact::Library::loadSignal(sig, NodePayloadL().first);
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

            ImGui::SameLine();
            ImGui::Button("",ImVec2(10,0));

            ImGui::PushItemWidth(-1);

            ImGui::SameLine(43);
            float v = (float)gui->device->session->getVolume(i);
            if (ImGui::MiniSliderFloat(str("##Volume", i).c_str(), &v, 0, 1, true))
                gui->device->session->setVolume(i, v);
            if (ImGui::IsItemClicked(1))
            {
                v = v == 0.0f ? 1.0f : v == 1.0f ? 0.0f : v < 0.5f ? 0.0f : 1.0f;
                gui->device->session->setVolume(i, v);
            }

            ImGui::SameLine(43);
            float p = (float)gui->device->session->getPitch(i);
            p = std::log10(p);
            if (ImGui::MiniSliderFloat(str("##Pitch", i).c_str(), &p, -1, 1, false))
                gui->device->session->setPitch(i, std::pow(10, p));
            if (ImGui::IsItemClicked(1))
            {
                gui->device->session->setPitch(i, 1);
            }

            ImGui::PopItemWidth();
            ImGui::PopID();
        }
    }
    ImGui::PopStyleVar();
    ImGui::EndChild();
}

void Player::playCh(int ch)
{
    tact::Signal sig;
    if (gui->workspace->activeTab == Workspace::TabSequencer)
        sig = gui->workspace->sequencer.buildSignal();
    else
        sig = gui->workspace->designer.buildSignal();
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
}