#include "Player.hpp"
#include "Gui.hpp"
#include "Custom.hpp"

using namespace mahi::gui;

Player::Player(Gui& gui) : Widget(gui)
{
    gui.device.onSessionOpen.connect(this, &Player::rechannel);
    rechannel();
}

void Player::update()
{
    ImGui::BeginFixed("Player", position, size, ImGuiWindowFlags_NoTitleBar);
    ImGui::BeginGroup(); // help group
    BeginPulsable(false,true);
    if (ImGui::Button(ICON_FA_PLAY, ImVec2(25, 0)))
        playSelected();
    EndPulsable();
    if (SignalTarget()) {
        tact::Signal sig;
        if (tact::Library::loadSignal(sig, SignalPayload().first))
        {
            for (int i = 0; i < gui.device.session->getCurrentDevice().maxChannels; ++i)
                gui.device.session->play(i,sig);
        }
    }
    if (ImGui::IsItemClicked(1)) {
        for (int i = 0; i < gui.device.session->getCurrentDevice().maxChannels; ++i)
            gui.device.session->stop(i);
    }
    ImGui::SameLine();

    float xRem = ImGui::GetContentRegionAvail().x;
    ImGui::PushItemWidth(((xRem-4) * 0.5f));
    if (ImGui::SliderFloat("##MasterVolume", &m_masterVol, 0, 1, "V"))
    {
        for (int i = 0; i < gui.device.session->getCurrentDevice().maxChannels; ++i)
            gui.device.session->setVolume(i, m_masterVol);
    }
    if (ImGui::IsItemClicked(1))
    {
        m_masterVol = m_masterVol == 0.0f ? 1.0f : m_masterVol == 1.0f ? 0.0f : m_masterVol < 0.5f ? 0.0f : 1.0f;
        for (int i = 0; i < gui.device.session->getCurrentDevice().maxChannels; ++i)
            gui.device.session->setVolume(i, m_masterVol);
    }
    ImGui::SameLine();
    if (ImGui::SliderFloat("##MasterPitch", &m_masterPitch, -1, 1, "P"))
    {
        float pitch = std::pow(10, m_masterPitch);
        for (int i = 0; i < gui.device.session->getCurrentDevice().maxChannels; ++i)
            gui.device.session->setPitch(i, pitch);
    }
    if (ImGui::IsItemClicked(1))
    {
        m_masterPitch = 0;
        for (int i = 0; i < gui.device.session->getCurrentDevice().maxChannels; ++i)
            gui.device.session->setPitch(i, 1);
    }
    ImGui::PopItemWidth();
    ImGui::Separator();
    updateChannels();
    ImGui::EndGroup();
    
    if (HelpTarget()) 
        ImGui::OpenPopup("Player Help");
    if (ImGui::BeginHelpPopup("Player Help")) {
        ImGui::BulletText("Left-click the " ICON_FA_PLAY " or numbered buttons to play the currently designed Signal on all or individual channels");
        ImGui::BulletText("Right-click the " ICON_FA_PLAY " or numbered buttons to stop Signals on all or individual channels");
        ImGui::BulletText("Drag Signals from the Library tab on to the " ICON_FA_PLAY " and numbered buttons to play them");
        ImGui::BulletText("Use the left and right sliders to adjust volume and pitch, respectively");
        ImGui::BulletText("Right-click sliders to quickly toggle positions");
        ImGui::BulletText("Ctrl+Left-click sliders for numeric text entry");
        ImGui::BulletText("Channels currently in the Spatializer will be highlighted and locked");
        ImGui::EndPopup();
    }

    ImGui::End();
}

/// Updates the channel selection checkboxes
void Player::updateChannels()
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {ImGui::GetStyle().FramePadding.x, 0});
    ImGui::BeginChild("Channels", ImVec2(0, 0), false, ImGuiWindowFlags_NoBackground);
    if (gui.device.session) {
        for (int i = 0; i < gui.device.session->getCurrentDevice().maxChannels; ++i)
        {
            ImGui::PushID(i);
            bool playing = gui.device.session->isPlaying(i);
            if (playing)
                ImGui::PushStyleColor(ImGuiCol_Button, Grays::Gray50);
            auto label = std::to_string(i);
            bool inSpat = gui.workspace.spatializer.spatializer.hasChannel(i);
            if (inSpat)
                ImGui::PushStyleColor(ImGuiCol_Text, gui.theme.spatializerColor);
            BeginPulsable(false,true);
            if (ImGui::Button(label.c_str(), ImVec2(25, 0)))
                playCh(i);
            EndPulsable();
            if (inSpat)
                ImGui::PopStyleColor();
            if (playing)
                ImGui::PopStyleColor();

            if (SignalTarget()) {
                tact::Signal sig;
                tact::Library::loadSignal(sig, SignalPayload().first);
                gui.device.session->play(i, sig);
            }
            if (ImGui::IsItemClicked(1))
                gui.device.session->stop(i);
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                m_payload = i;
                ImGui::SetDragDropPayload("DND_CHANNEL", &m_payload, sizeof(int));
                ImGui::Text(label.c_str());
                ImGui::EndDragDropSource();
            }

            ImGui::SameLine();

            ImGui::BeginDisabled(inSpat);
            float xRem = ImGui::GetContentRegionAvail().x;
            float item_width = (xRem-4) * 0.5f;
            float item_height = ImGui::GetFrameHeight();
            ImGui::PushItemWidth(item_width);
            ImGui::SameLine();
            float v = (float)gui.device.session->getVolume(i);
            auto old_cpos = ImGui::GetCurrentWindow()->DC.CursorPos;
            ImGui::PushID(i);
            if (ImGui::SliderFloat("##Volume", &v, 0, 1, ""))
                gui.device.session->setVolume(i, v);
            ImGui::PopID();
            auto new_cpos = ImGui::GetCurrentWindow()->DC.CursorPos;
            if (ImGui::IsItemClicked(1))
            {
                v = v == 0.0f ? 1.0f : v == 1.0f ? 0.0f : v < 0.5f ? 0.0f : 1.0f;
                gui.device.session->setVolume(i, v);
            }
            float level = (float)gui.device.session->getLevel(i);
            level = ImClamp(level, 0.0f, 1.0f);
            float meter_width = level * (item_width - 4 - ImGui::GetStyle().GrabMinSize);
            if (level > 0.01f) {
                ImGui::GetCurrentWindow()->DC.CursorPos = old_cpos;
                ImGui::GetWindowDrawList()->AddRectFilled(old_cpos + ImVec2(2,2), old_cpos + ImVec2(meter_width,item_height-2), IM_COL32(255,200,0,255), ImGui::GetStyle().GrabRounding);
                ImGui::GetCurrentWindow()->DC.CursorPos = new_cpos;        
            }

            ImGui::SameLine();
            float p = (float)gui.device.session->getPitch(i);
            p = std::log10(p);
            ImGui::PushID(i);
            if (ImGui::SliderFloat("##Ptich", &p, -1, 1, ""))
                gui.device.session->setPitch(i, std::pow(10, p));
            ImGui::PopID();
            if (ImGui::IsItemClicked(1))            
                gui.device.session->setPitch(i, 1);
            ImGui::PopItemWidth();
            ImGui::EndDisabled();

            ImGui::PopID();
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleVar();
}

void Player::playCh(int ch)
{
    tact::Signal sig;
    if (gui.workspace.activeTab == Workspace::TabSequencer)
        sig = gui.workspace.sequencer.buildSignal();
    else
        sig = gui.workspace.designer.buildSignal();
    gui.device.session->play(ch, sig);
}

/// Plays all selected channels
void Player::playSelected()
{
    for (int i = 0; i < gui.device.session->getCurrentDevice().maxChannels; ++i)
        playCh(i);
}

/// Gets number of channels from current Syntacts device and resizes accordingly
void Player::rechannel()
{
    int maxChannels = gui.device.session ? gui.device.session->getCurrentDevice().maxChannels : 0;
}