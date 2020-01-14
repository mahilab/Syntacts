#include "Library.hpp"
#include "Gui.hpp"
#include "ImGui/Custom.hpp"
#include <ImGui/imgui_demo.cpp>

using namespace carnot;

Library::Library(Gui *gui) : Widget(gui),
                             palette(gui)
{
}

const std::string& Library::getSelectedName() {
    static std::string nothing = "";
    if (m_lib.count(m_selected))
        return m_selected;
    return nothing;
}

tact::Signal Library::getSelectedSignal()
{
    if (m_lib.count(m_selected))
    {
        tact::Signal sig;
        tact::Library::loadSignal(sig, m_selected);
        return sig;
    }
    return tact::Signal();
}

void Library::start()
{
    Engine::onFileDrop.connect(this, &Library::onFileDrop);
    fs::create_directories(tact::Library::getLibraryDirectory());
    sync();
    if (m_lib.size() > 0)
        m_selected = m_lib.begin()->first;
    startCoroutine(periodicSync());
}

Enumerator Library::periodicSync()
{
    while (true)
    {
        co_yield new WaitForSeconds(1.0f);
        sync();
    }
}

void Library::sync()
{
    // load new entries from disk
    std::set<std::string> diskNames;
    for (const auto &dir_entry : fs::directory_iterator(tact::Library::getLibraryDirectory()))
    {
        if (dir_entry.path().has_extension() && dir_entry.path().extension() == ".sig")
        {
            std::string name = dir_entry.path().stem().generic_string();
            diskNames.insert(name);
            if (m_lib.count(name) == 0)
            {
                m_lib[name] = Entry();
                m_lib[name].name = name;
                m_lib[name].path = dir_entry.path();
            }
        }
    }
    // remove stale entries from memory
    for (auto it = m_lib.cbegin(); it != m_lib.cend(); /**/)
    {
        if (diskNames.count(it->second.name) == 0)
            m_lib.erase(it++);
        else
            it++;
    }
}

void Library::update()
{
    render();
}

void Library::render()
{
    ImGui::NodeUpdate();
    ImGui::BeginFixed(getName().c_str(), rect.getPosition(), rect.getSize());
    if (ImGui::BeginTabBar("LibraryWindowTabs"))
    {
        if (ImGui::BeginTabItem("Palette##Tab"))
        {
            ImGui::BeginGroup();
            palette.render();
            ImGui::EndGroup();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Library ##Tab"))
        {
            ImGui::BeginGroup();
            renderCreateDialog();
            ImGui::Separator();
            renderLibraryList();
            ImGui::Separator();
            renderLibraryControls();
            ImGui::EndGroup();
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload *playload = ImGui::AcceptDragDropPayload("DND_HELP"))
                {
                    print("TODO");
                }
                ImGui::EndDragDropTarget();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void Library::renderCreateDialog()
{
    ImGui::PushItemWidth(-30);
    bool entered = ImGui::InputText("##SignalName", m_inputBuffer, 64, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::PopItemWidth();
    ImGui::SameLine();
    std::string filename(m_inputBuffer);
    bool valid = filename != "";

    ImGui::BeginDisabled(!valid);
    if (ImGui::Button(ICON_FA_PLUS_SQUARE) || (entered && valid))
    {
        if (!m_lib.count(filename))
        {
            auto cue = gui->workspace->designer.buildSignal();
            tact::Library::saveSignal(cue, filename);
            sync();
            gui->status->pushMessage("Created Signal " + filename);
            memset(m_inputBuffer, 0, 64);
            m_selected = filename;
        }
        else
        {
            gui->status->pushMessage("Signal " + filename + " already exists", StatusBar::Error);
        }
    }
    gui->status->showTooltip("Create new Signal");
    ImGui::EndDisabled(!valid);
}
void Library::renderLibraryList()
{
    auto avail = ImGui::GetContentRegionAvail();
    ImGui::PushStyleColor(ImGuiCol_ChildBg, Color::Transparent);
    ImGui::BeginChild("CueList", ImVec2(0, avail.y - 29));
    for (auto &pair : m_lib)
    {
        Entry &entry = pair.second;
        if (ImGui::Selectable(entry.name.c_str(), m_selected == entry.name))
            m_selected = entry.name;
        ImGui::NodeSourceL(entry.name);
        if (ImGui::BeginPopupContextItem())
        {
            static std::vector<ImVec2> points(10000);
            tact::Signal sig;
            tact::Library::loadSignal(sig, entry.name);
            ImGui::PlotSignal(entry.name.c_str(), sig, points, Blues::DeepSkyBlue, 1.0f, ImVec2(300,150), false);
            ImGui::EndPopup();
        }
        if (ImGui::IsItemHovered()) {            
            tact::Signal sig;
            tact::Library::loadSignal(sig, entry.name);
            gui->visualizer->setRenderedSignal(sig, Blues::DeepSkyBlue);
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void Library::renderLibraryControls()
{
    bool disabled = m_lib.count(m_selected) == 0;
    ImGui::BeginDisabled(disabled);
    float space = 27;
    if (ImGui::Button(ICON_FA_SAVE))
    {
        auto cue = gui->workspace->designer.buildSignal();
        tact::Library::saveSignal(cue, m_selected);
        gui->status->pushMessage("Saved Signal " + m_selected);
        sync();
    }
    gui->status->showTooltip("Save Selected Signal");

    ImGui::SameLine(1 * space);
    if (ImGui::Button(ICON_FA_TRASH))
    {
        gui->status->pushMessage("Deleted Signal " + m_selected);
        auto it = m_lib.find(m_selected);
        auto next = std::next(it);
        auto prev = it == m_lib.begin() ? it : std::prev(it);
        if (next != m_lib.end())
            m_selected = next->second.name;
        else if (prev != m_lib.end())
            m_selected = prev->second.name;
        else
            m_selected = "";
        // delete
        fs::remove(it->second.path);

        sync();
    }
    gui->status->showTooltip("Delete Selected Signal");

    ImGui::SameLine(2 * space);
    if (ImGui::Button(ICON_FA_MUSIC))
    {
        std::string filePath = m_selected;
        tact::Library::exportSignal(getSelectedSignal(), filePath, tact::FileFormat::WAV);
        gui->status->pushMessage("Exported Signal " + m_selected + " to " + filePath + ".wav");
    }
    gui->status->showTooltip("Export Selected Signal to WAV");

    ImGui::SameLine(3 * space);
    if (ImGui::Button(ICON_FA_TABLE))
    {
        std::string filePath = m_selected;
        tact::Library::exportSignal(getSelectedSignal(), filePath, tact::FileFormat::CSV);
        gui->status->pushMessage("Exported Signal " + m_selected + " to " + filePath + ".csv");
    }
    gui->status->showTooltip("Export Selected Signal to CSV");

    ImGui::SameLine(4 * space);
    if (ImGui::Button(ICON_FA_CODE))
    {
        std::string filePath = m_selected;
        tact::Library::exportSignal(getSelectedSignal(), filePath, tact::FileFormat::JSON);
        gui->status->pushMessage("Exported Signal " + m_selected + " to " + filePath + ".json");
    }
    gui->status->showTooltip("Export Selected Signal to JSON");

    ImGui::EndDisabled(disabled);
}

void Library::onFileDrop(const std::string& filePath, const carnot::Vector2u& pos) {
    tact::Signal imported;
    auto path = fs::path(filePath);
    std::string filename = path.filename().string();
    if (tact::Library::importSignal(imported, filePath))
    {
        tact::Library::saveSignal(imported, path.stem().string());
        gui->status->pushMessage("Imported " + filename);

    }
    else {
        gui->status->pushMessage("Failed to import " + filename, StatusBar::Error);
    }
}
