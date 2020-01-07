#include "StatusBar.hpp"
#include "ImGui/Custom.hpp"
#include "Gui.hpp"

using namespace carnot;

void StatusBar::pushMessage(const std::string &text, InfoLevel level)
{
    m_fadeTime = 0;
    static Color infoColors[3] = {ImGui::GetStyle().Colors[ImGuiCol_Text], Yellows::Khaki, Reds::LightCoral};
    m_fadeColors[0.00f] = Color::Transparent;
    m_fadeColors[0.02f] = infoColors[level];
    m_fadeColors[0.80f] = infoColors[level];
    m_fadeColors[1.00f] = Color::Transparent;
    m_notification = text;
}

void StatusBar::showTooltip(const std::string &tooltip)
{
    if (ImGui::IsItemHovered() && m_fadeTime > m_fadeDuration)
    {
        m_tooltip = tooltip;
    }
}

StatusBar::StatusBar(Gui *gui) : Widget(gui)
{
}

void StatusBar::start()
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
    render();
}

void StatusBar::render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(7, 7));
    ImGui::BeginFixed("StatusBar", rect.getPosition(), rect.getSize());
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
        m_fadeTime += Engine::deltaTime();
        float t = Math::clamp01(m_fadeTime / m_fadeDuration);
        auto color = m_fadeColors(t);
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::LabelText("##InfoText", m_notification.c_str());
        ImGui::PopStyleColor();
    }
}

void StatusBar::renderButtons()
{
    bool modalOpen = true;

#ifndef TACT_USE_MALLOC
    ImGui::SameLine(ImGui::GetWindowWidth() - 365);
#else
    ImGui::SameLine(ImGui::GetWindowWidth() - 285);
#endif
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, m_cpuGradient(Math::clamp01(m_cpuLoad)));
    if (gui->device->session)
        m_cpuLoad = (float)gui->device->session->getCpuLoad();
    ImGui::ProgressBar(m_cpuLoad, ImVec2(100, 0));
    ImGui::PopStyleColor();
    showTooltip("Session CPU Thread Load");

#ifndef TACT_USE_MALLOC
    ImGui::SameLine();
    std::string used = str(tact::Signal::pool().blocksUsed()) + "/" + str(tact::Signal::pool().blocksTotal());
    if (ImGui::Button(used.c_str(), ImVec2(70, 0)))
        ImGui::OpenPopup("Signal Memory Pool");
    if (ImGui::BeginPopupModal("Signal Memory Pool", &modalOpen, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        auto occupied = tact::Signal::pool().blocksOccupied();
        int i = 0;
        int rows = 16;
        int cols = tact::MAX_SIGNALS / rows;
        std::string rowText(tact::MAX_SIGNALS / 16, ' ');

        for (int r = 0; r < rows; ++r)
        {
            for (int c = 0; c < cols; ++c)
            {
                if (occupied[i++])
                    rowText[c] = 'X';
                else
                    rowText[c] = '_';
            }
            ImGui::Text(rowText.c_str());
        }
        ImGui::EndPopup();
    }
    showTooltip("View Signal Memory Pool Fragmentation");
#else
    ImGui::SameLine();
    ImGui::Button(str(tact::Signal::count()).c_str(), ImVec2(30, 0));
    showTooltip("Global Signal Count");
#endif

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_GITHUB))
        openUrl("https://github.com/mahilab/Syntacts");
    showTooltip("Open GitHub Repository");

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_BUG))
        openUrl("https://github.com/mahilab/Syntacts/issues");
    showTooltip("Open Issue Tracker");

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_ENVELOPE "##Brandon"))
        openEmail("mep9@rice.edu; btc6@rice.edu", "Syntacts");
    showTooltip("Email Feedback");

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_PALETTE))
    {
        pushMessage("This doesn't work yet :(");
    }
    showTooltip("Customize Theme");

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_QUESTION))
        ImGui::OpenPopup("Help");
    showTooltip("Show Help Info");
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        static int helpPayload = 1;
        ImGui::SetDragDropPayload("DND_HELP", &helpPayload, sizeof(int));
        ImGui::Text(ICON_FA_QUESTION);
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginPopupModal("Help", &modalOpen, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 250);
        ImGui::TextWrapped("To get helpful information and tips, simply drag the " ICON_FA_QUESTION " button over areas of interest.");
        ImGui::PopTextWrapPos();
        ImGui::EndPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_INFO))
        ImGui::OpenPopup("Syntacts GUI v1.0.0");
    showTooltip("Show General Info");
    if (ImGui::BeginPopupModal("Syntacts GUI v1.0.0", &modalOpen, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        ImGui::Text("MAHI Lab");
        ImGui::SameLine(150);
        if (ImGui::Button(ICON_FA_HOME "##MAHI"))
            openUrl("https://mahilab.rice.edu/");
        ImGui::SameLine(175);
        if (ImGui::Button(ICON_FA_GITHUB "##MAHI"))
            openUrl("https://github.com/mahilab");

        ImGui::Text("Evan Pezent");
        ImGui::SameLine(150);
        if (ImGui::Button(ICON_FA_HOME "##Evan"))
            openUrl("http://www.evanpezent.com");
        ImGui::SameLine(125);
        if (ImGui::Button(ICON_FA_ENVELOPE "##Evan"))
            openEmail("epezent@rice.edu", "Syntacts");
        ImGui::SameLine(175);
        if (ImGui::Button(ICON_FA_GITHUB "##Evan"))
            openUrl("https://github.com/epezent");
        ImGui::EndPopup();
    }
}