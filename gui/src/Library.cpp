#include "Library.hpp"
#include "Gui.hpp"
#include "ImGui/Custom.hpp"
#include <filesystem>

using namespace carnot;
namespace fs = std::filesystem;

namespace {
    template <class Class, class RR, class... Args2>
    auto memberBind(Class *object, RR (Class::*method)(Args2...))
    {
        auto f = [object, method](Args2... args) { return (object->*method)(args...); };
        return f;
    }
}


Library::Library(Gui *gui) : Widget(gui),
                             m_watcher(tact::Library::getLibraryDirectory(), 1000),
                             palette(gui)
{
}

tact::Signal Library::getSelectedSignal()
{
    if (m_lib.count(m_selected))
    {
        if (!m_lib[m_selected].loaded)
            tact::Library::loadSignal(m_lib[m_selected].disk, m_selected);
        return m_lib[m_selected].disk;
    }
    return tact::Signal();
}

void Library::onFileDrop(const std::string& filePath, const carnot::Vector2u& pos) {
    tact::Signal imported;
    auto path = fs::path(filePath);
    std::string filename = path.stem().string();
    if (tact::Library::importSignal(imported, filePath))
    {
        if (m_lib.count(filename))
            m_ignoreFilChange = true;
        tact::Library::saveSignal(imported, filename);
        gui->status->pushMessage("Imported " + filename);
        std::lock_guard<std::mutex> lock(m_mtx);
        m_lib[filename] = Entry();
        m_lib[filename].disk = imported;
        m_lib[filename].name = filename;
        m_lib[filename].loaded = true;
    }
    else {
        gui->status->pushMessage("Failed to import " + filename, StatusBar::Error);
    }
}

// function called when disk files change
void Library::onFileChange(std::string path, FileStatus status) {
    std::lock_guard<std::mutex> lock(m_mtx);
    if (m_ignoreFilChange) {
        m_ignoreFilChange = false;
        return;
    }
    fs::path p(path);
    if (p.extension() == ".sig") {
        std::string name = p.stem().string();
        if (status == FileStatus::Created) {
            if (m_lib.count(name) == 0)
            {
                m_lib[name] = Entry();
                m_lib[name].name = name;
                m_lib[name].loaded = false;
                gui->status->pushMessage("Signal " + name + " added from disk");
            }
        }
        else if (status == FileStatus::Erased)  {
            m_lib.erase(name);
            gui->status->pushMessage("Signal " + name + " deleted from disk");
        }
        else if (status == FileStatus::Modified) {
            if (m_lib.count(name)) {
                m_lib[name].loaded = false;
                gui->status->pushMessage("Signal " + name + " modified on disk");
            }
        }
    }
}

void Library::start()
{
    auto& libPath = tact::Library::getLibraryDirectory();
    // make sure the lib directory exists
    fs::create_directories(libPath);
    // initialize library listing
    for (const auto &dir_entry : fs::directory_iterator(libPath))
    {
        if (dir_entry.path().has_extension() && dir_entry.path().extension() == ".sig")
        {
            std::string name = dir_entry.path().stem().string();
            if (m_lib.count(name) == 0)
            {
                m_lib[name] = Entry();
                m_lib[name].name = name;
            }
        }
    }
    // set selected entry
    if (m_lib.size() > 0)
        m_selected = m_lib.begin()->first;

 
    /// start FileWatcher
    auto tf = [&]() { m_watcher.start(memberBind(this,&Library::onFileChange)); };
    std::thread t(tf);
    t.detach();

    // connect onFileDrop to carnot
    Engine::onFileDrop.connect(this, &Library::onFileDrop);
}

void Library::update()
{
    std::lock_guard<std::mutex> lock(m_mtx);
    render();
}

void Library::render()
{
    ImGui::BeginFixed(getName().c_str(), rect.getPosition(), rect.getSize());
    if (ImGui::BeginTabBar("LibraryWindowTabs"))
    {
        if (ImGui::BeginTabItem(" Palette ##Tab"))
        {
            ImGui::BeginGroup();
            palette.render();
            ImGui::EndGroup();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(" Library ##Tab"))
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
            tact::Signal sig;
            if (gui->workspace->activeTab == Workspace::TabSequencer)
                sig = gui->workspace->sequencer.buildSignal();
            else
                sig = gui->workspace->designer.buildSignal();
            tact::Library::saveSignal(sig, filename);
            m_lib[filename] = Entry();
            m_lib[filename].disk = sig;
            m_lib[filename].loaded = true;
            m_lib[filename].name = filename;
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
    ImGui::BeginChild("LibraryList", ImVec2(0, avail.y - 29));
    for (auto &pair : m_lib)
    {
        Entry &entry = pair.second;
        if (ImGui::Selectable(entry.name.c_str(), m_selected == entry.name))
            m_selected = entry.name;
        if (ImGui::IsItemHovered()) {            
            if (!entry.loaded) {            
               if (tact::Library::loadSignal(entry.disk, entry.name))
                    entry.loaded = true;
            }
            gui->visualizer->setRenderedSignal(entry.disk, Blues::DeepSkyBlue);
        }
        SignalSource(entry.name, entry.disk);

    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void Library::renderLibraryControls()
{
    bool disabled = m_lib.count(m_selected) == 0;
    ImGui::BeginDisabled(disabled);

    // SAVE

    float space = 27;
    if (ImGui::Button(ICON_FA_SAVE))
    {
        tact::Signal sig;
        if (gui->workspace->activeTab == Workspace::TabSequencer)
            sig = gui->workspace->sequencer.buildSignal();
        else
            sig = gui->workspace->designer.buildSignal();
        tact::Library::saveSignal(sig, m_selected);
        m_lib[m_selected].disk = sig;
        m_lib[m_selected].loaded = true;
        m_ignoreFilChange = true;
        gui->status->pushMessage("Saved Signal " + m_selected);
    }
    gui->status->showTooltip("Save Selected Signal");

    // DELETE

    ImGui::SameLine(1 * space);
    if (ImGui::Button(ICON_FA_TRASH))
    {
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
        tact::Library::deleteSignal(it->second.name);
        m_lib.erase(it);
        gui->status->pushMessage("Deleted Signal " + m_selected);
        m_ignoreFilChange = true;
    }
    gui->status->showTooltip("Delete Selected Signal");

    // EXPORT

    ImGui::SameLine(2 * space);
    if (ImGui::Button(ICON_FA_MUSIC))
    {
        tact::Library::exportSignal(getSelectedSignal(), m_selected, tact::FileFormat::WAV);
        gui->status->pushMessage("Exported Signal " + m_selected + " to " + m_selected + ".wav");
    }
    gui->status->showTooltip("Export Selected Signal to WAV");

    ImGui::SameLine(3 * space);
    if (ImGui::Button(ICON_FA_TABLE))
    {
        tact::Library::exportSignal(getSelectedSignal(), m_selected, tact::FileFormat::CSV);
        gui->status->pushMessage("Exported Signal " + m_selected + " to " + m_selected + ".csv");
    }
    gui->status->showTooltip("Export Selected Signal to CSV");

    ImGui::SameLine(4 * space);
    if (ImGui::Button(ICON_FA_CODE))
    {
        tact::Library::exportSignal(getSelectedSignal(), m_selected, tact::FileFormat::JSON);
        gui->status->pushMessage("Exported Signal " + m_selected + " to " + m_selected + ".json");
    }
    gui->status->showTooltip("Export Selected Signal to JSON");

    ImGui::EndDisabled(disabled);
}
