#include "Spatializer.hpp"
#include "Gui.hpp"

using namespace carnot;

Spatializer::Spatializer(Gui* gui) :
    Widget(gui)
{
    m_target.pos.x = m_spatializer.getTarget().x;
    m_target.pos.y = m_spatializer.getTarget().y;
    m_target.radius =m_spatializer.getRadius();
    gui->device->onSessionInit.connect(this, &Spatializer::onSessionInit);

}

void Spatializer::render()
{
    ImGui::Spatializer("Spatializer##Grid", m_target, m_channels, 10, Greens::Chartreuse, ImVec2(260, -1), "DND_CHANNEL", m_divs[0], m_1d ? 1 : m_divs[1], m_snap);
    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::PushItemWidth(200);
    ImGui::Text(m_1d ? "Divisions" : "X Divisions");
    if (!m_1d) {
        ImGui::SameLine(102);
        ImGui::Text("Y Divisions");
    }
    ImGui::SameLine(220);
    ImGui::Text("Snap");
    ImGui::SameLine(260);
    ImGui::Text("1D");
    if (m_1d)
        ImGui::SliderInt("##Grid Size", m_divs, 1, 25);
    else
        ImGui::SliderInt2("##Grid Size", m_divs, 1, 25);
    ImGui::SameLine(220);
    ImGui::Checkbox("##Snap", &m_snap);
    ImGui::SameLine(260);
    ImGui::Checkbox("##1D", &m_1d);
    if (ImGui::Button("Fill", ImVec2(200, 0)))
        fillGrid();
    if (ImGui::Button("Clear", ImVec2(200, 0)))
        m_channels.clear();
    ImGui::PushItemWidth(175);

    bool entered = ImGui::InputText("##SignalName", m_inputBuffer, 64, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::Button(ICON_FA_SAVE);

    ImGui::Separator();
    if (m_1d)
        ImGui::DragFloat("Position", &m_target.pos.x, 0.005f, 0.0f, 1.0f);
    else
        ImGui::DragFloat2("Position", &m_target.pos.x, 0.005f, 0.0f, 1.0f);
    ImGui::DragFloat("Radius", &m_target.radius, 0.005f, 0.0f, 1.0f);
    if (ImGui::BeginCombo("Roll-Off", "Lograthmic"))
    {
        ImGui::EndCombo();
    }

    ImGui::Separator();
    ImGui::NodeSlot(m_sigName.c_str(), ImVec2(200, 0));
    if (ImGui::NodeDroppedL()) {
        m_sigName = ImGui::NodePayloadL();
        tact::Library::loadSignal(signal, m_sigName);
    }
    ImGui::SameLine();
    ImGui::Text("Signal");
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y * 2));
    if (ImGui::Button(ICON_FA_PLAY, ImVec2(25,0))) {
        for (auto& pair : m_channels) 
            print(pair.first, pair.second.pos.x, pair.second.pos.y);
    }
    ImGui::SameLine();
    ImGui::Button(ICON_FA_PAUSE);
    ImGui::SameLine();
    ImGui::Button(ICON_FA_STOP);
    ImGui::SameLine(75);
    ImGui::PushItemWidth(125);
    ImGui::MiniSliderFloat("##Volume", &m_volume, 0, 1, true);
    ImGui::SameLine(75);
    ImGui::MiniSliderFloat("##Pitch", &m_pitch, -1, 1, false);
    ImGui::PopItemWidth();
    ImGui::PopStyleVar();
    ImGui::PopItemWidth();
    ImGui::EndGroup();
}

void Spatializer::onSessionInit() {
    m_spatializer.bind(gui->device->session.get());
}

void Spatializer::update() {

}

void Spatializer::fillGrid() {
        m_channels.clear();
        if (!gui->device->session)
            return;
        int chs = gui->device->session->getChannelCount();
        
        float div[2] = {1.0f / (m_divs[0]-1), 1.0f / (m_divs[1]-1)};

        int k = 0;
        rowsFirst = !rowsFirst;

        if (rowsFirst)
        {
            for (int j = 0; j < m_divs[0]; ++j)
            {
                for (int i = 0; i < m_divs[1]; ++i)
                {
                    ImGui::SpatializerNode node;
                    node.index = k;
                    node.pos.x = i * div[0];
                    node.pos.y = j * div[1];
                    node.held = false;
                    m_channels[k] = node;
                    if (++k == chs)
                        return;
                }
            }
        }
        else
        {
            for (int i = 0; i < m_divs[1] + 1; ++i)
            {
                for (int j = 0; j < m_divs[0] + 1; ++j)
                {
                    ImGui::SpatializerNode node;
                    node.index = k;
                    node.pos.x = i * div[1];
                    node.pos.y = j * div[0];
                    node.held = false;
                    m_channels[k] = node;                    
                    if (++k == chs)
                        return;                
                }
            }
        }
    }