#include "StatusBar.hpp"
#include "Custom.hpp"
#include "Gui.hpp"
#include "Theme.hpp"
#include "Gui.hpp"

using namespace mahi::gui;

void StatusBar::pushMessage(const std::string &text, InfoLevel level)
{
    Color textCol = ImGui::GetStyle().Colors[ImGuiCol_Text];
    m_fadeTime = 0;
    m_fadeColors[0.00f] = {0,0,0,0};
    m_fadeColors[0.02f] = level == InfoLevel::Info ? textCol : level == Warning ? Yellows::Khaki : Reds::LightCoral;
    m_fadeColors[0.80f] = level == InfoLevel::Info ? textCol : level == Warning ? Yellows::Khaki : Reds::LightCoral;
    m_fadeColors[1.00f] = {0,0,0,0};
    m_notification = text;
}

void StatusBar::showTooltip(const std::string &tooltip)
{
    if (ImGui::IsItemHovered() && m_fadeTime > m_fadeDuration)
    {
        m_tooltip = tooltip;
    }
}

StatusBar::StatusBar(Gui& gui) : Widget(gui)
{
    pushMessage("Welcome to Syntacts!");
    m_cpuGradient[0.00f] = ImGui::GetStyle().Colors[ImGuiCol_PlotHistogram];
    m_cpuGradient[0.25f] = ImGui::GetStyle().Colors[ImGuiCol_PlotHistogram];
    m_cpuGradient[0.50f] = Yellows::Gold;
    m_cpuGradient[0.75f] = Oranges::OrangeRed;
    m_cpuGradient[1.00f] = Reds::DarkRed;
}

void StatusBar::update()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(7, 7));
    ImGui::BeginFixed("StatusBar", position, size, ImGuiWindowFlags_NoTitleBar);
    renderText();
    renderButtons();
    ImGui::End();
    ImGui::PopStyleVar();
}

void StatusBar::renderText()
{
    if (m_tooltip != "")
    {
        ImGui::LabelText("##InfoText", m_tooltip.c_str());
        m_tooltip = "";
    }
    else
    {
        m_fadeTime += ImGui::GetIO().DeltaTime;
        float t = mahi::util::clamp01(m_fadeTime / m_fadeDuration);
        auto color = m_fadeColors(t);
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::LabelText("##InfoText", m_notification.c_str());
        ImGui::PopStyleColor();
    }
}

void StatusBar::renderButtons()
{
    bool modalOpen = true;

    ImGui::SameLine(ImGui::GetWindowWidth() - 255);
    double cpu = 0;
    if (gui.device.session)
        cpu = gui.device.session->getCpuLoad();
    float rounded = (int)(cpu * 100.0f)/100.0f;
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, m_cpuGradient(mahi::util::clamp01((float)cpu)));
    ImGui::ProgressBar(rounded, ImVec2(100, 0));
    ImGui::PopStyleColor();
    showTooltip("Session CPU Thread Load");

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_HOME)) {
        mahi::gui::open_url("https://www.syntacts.org");
    }
    showTooltip("Open Syntacts Website");
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_GITHUB)) {
        mahi::gui::open_url("https://github.com/mahilab/Syntacts");
    }
    showTooltip("Open GitHub Repository");

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_BUG))
        gui.debug.show = true;
    showTooltip("Display Debug Info");

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_ADJUST)) 
    {
        if (ImGui::GetIO().KeyCtrl)
            gui.theme.showEditor = true;
        else
            gui.theme.cycle();
    }
    showTooltip("Change Themes");
    

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_QUESTION))
        ImGui::OpenPopup("Help");
    showTooltip("Show Help Info");
    HelpSource();
    

    if (ImGui::BeginPopupModal("Help", &modalOpen, ImGuiWindowFlags_NoResize))
    {
        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 250);
        ImGui::TextWrapped("To get helpful information and tips, simply drag the " ICON_FA_QUESTION " button over areas of interest.");
        ImGui::PopTextWrapPos();
        ImGui::EndPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_INFO))
        ImGui::OpenPopup("Syntacts GUI");
    showTooltip("Show General Info");
    if (ImGui::BeginPopupModal("Syntacts GUI", &modalOpen, ImGuiWindowFlags_NoResize))
    {
        ImGui::Text("MAHI Lab");
        ImGui::SameLine(150);
        if (ImGui::Button(ICON_FA_HOME "##MAHI")) {
            mahi::gui::open_url("https://mahilab.rice.edu/");
        }
        ImGui::SameLine(175);
        if (ImGui::Button(ICON_FA_GITHUB "##MAHI")) {
            mahi::gui::open_url("https://github.com/mahilab");
        }

        ImGui::Text("Evan Pezent");
        ImGui::SameLine(150);
        if (ImGui::Button(ICON_FA_HOME "##Evan")) {
            mahi::gui::open_url("http://www.evanpezent.com");
        }
        ImGui::SameLine(125);
        if (ImGui::Button(ICON_FA_ENVELOPE "##Evan")) {
            mahi::gui::open_email("epezent@rice.edu", "Syntacts");
        }
        ImGui::SameLine(175);
        if (ImGui::Button(ICON_FA_GITHUB "##Evan")) {
            mahi::gui::open_url("https://github.com/epezent");
        }
        ImGui::EndPopup();
    }
}



///////////////////////////////////////////////////////////////////////////////

/*
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
*/