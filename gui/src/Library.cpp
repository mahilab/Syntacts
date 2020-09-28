#include "Library.hpp"
#include "Gui.hpp"
#include "Custom.hpp"
#include <filesystem>

using namespace mahi::gui;
namespace fs = std::filesystem;

namespace {
    template <class Class, class RR, class... Args2>
    auto memberBind(Class *object, RR (Class::*method)(Args2...))
    {
        auto f = [object, method](Args2... args) { return (object->*method)(args...); };
        return f;
    }
}


Library::Library(Gui& gui) : Widget(gui),
                             m_watcher(tact::Library::getLibraryDirectory(), 1000),
                             palette(gui)
{
    init();
    gui.on_file_drop.connect(this, &Library::onFileDrop);
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

void Library::onFileDrop(const std::vector<std::string>& paths) {
    for (auto& p : paths) {
        tact::Signal imported;
        auto path = fs::path(p);
        std::string filename = path.stem().string();
        if (tact::Library::importSignal(imported, p))
        {
            if (m_lib.count(filename))
                m_ignoreFilChange = true;
            tact::Library::saveSignal(imported, filename);
            gui.status.pushMessage("Imported " + filename);
            std::lock_guard<std::mutex> lock(m_mtx);
            m_lib[filename] = Entry();
            m_lib[filename].disk = imported;
            m_lib[filename].name = filename;
            m_lib[filename].loaded = true;
        }
        else {
            gui.status.pushMessage("Failed to import " + filename, StatusBar::Error);
        }
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
                gui.status.pushMessage("Signal " + name + " added from disk");
            }
        }
        else if (status == FileStatus::Erased)  {
            m_lib.erase(name);
            gui.status.pushMessage("Signal " + name + " deleted from disk");
        }
        else if (status == FileStatus::Modified) {
            if (m_lib.count(name)) {
                m_lib[name].loaded = false;
                gui.status.pushMessage("Signal " + name + " modified on disk");
            }
        }
    }
}

void Library::init()
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
}

void Library::update()
{
    bool dummy = true;
    std::lock_guard<std::mutex> lock(m_mtx);
    ImGui::BeginFixed("Library", position, size, ImGuiWindowFlags_NoTitleBar);
    m_focused = false;

    if (ImGui::BeginTabBar("LibraryWindowTabs"))
    {
        if (ImGui::BeginTabItem(" Palette ##Tab"))
        {
            ImGui::BeginGroup();
            palette.update();
            ImGui::EndGroup();
            if (HelpTarget())
                ImGui::OpenPopup("Palette Help");
            if (ImGui::BeginHelpPopup("Palette Help")) {
                ImGui::BulletText("Drag items from the Palette to Node slots in the Designer tab");
                ImGui::BulletText("Palette items cannot be dragged on to Channels, the Sequencer, or the Spatializer");
                ImGui::EndPopup();
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(" Library ##Tab"))
        {
            m_focused = true; // ImGui::IsWindowFocused();

            ImGui::BeginGroup();
            renderCreateDialog();
            ImGui::Separator();
            renderLibraryList();

            ImGui::Separator();
            renderLibraryControls();
            ImGui::EndGroup();
            if (HelpTarget())
                ImGui::OpenPopup("Library Help");
            if (ImGui::BeginHelpPopup("Library Help")) {
                ImGui::BulletText("Use the input field and " ICON_FA_PLUS_SQUARE " button to save the currently Designed or Sequenced Signal");
                ImGui::BulletText("Signals are saved to the global Syntacts Library");
                ImGui::BulletText("Hover over a Library Signal for a preview or right-click it for additional controls");
                ImGui::BulletText("Use the buttons at the bottom to save, delete, and export Signals to different file formats");
                ImGui::BulletText("Right click items for more options");
                ImGui::BulletText("Drag files from your computer onto the GUI window to add them to the Library");
                ImGui::EndPopup();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}



void Library::renderCreateDialog()
{
    ImGui::PushItemWidth(-22);
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
            if (gui.workspace.activeTab == Workspace::TabSequencer)
                sig = gui.workspace.sequencer.buildSignal();
            else
                sig = gui.workspace.designer.buildSignal();
            tact::Library::saveSignal(sig, filename);
            m_lib[filename] = Entry();
            m_lib[filename].disk = sig;
            m_lib[filename].loaded = true;
            m_lib[filename].name = filename;
            gui.status.pushMessage("Created Signal " + filename);
            memset(m_inputBuffer, 0, 64);
            m_selected = filename;
        }
        else
        {
            gui.status.pushMessage("Signal " + filename + " already exists", StatusBar::Error);
        }
    }
    gui.status.showTooltip("Create new Signal");
    ImGui::EndDisabled();
}

void Library::renderLibraryList()
{
    auto avail = ImGui::GetContentRegionAvail();
    ImGui::PushStyleColor(ImGuiCol_ChildBg, {0,0,0,0});
    ImGui::BeginChild("LibraryList", ImVec2(0, avail.y - 29));
    m_focused = m_focused || ImGui::IsWindowFocused();
    for (auto &pair : m_lib)
    {
        Entry &entry = pair.second;
        if (ImGui::Selectable(entry.name.c_str(), m_selected == entry.name)) {
            m_selected = entry.name;
        }
        if (ImGui::IsItemHovered()) {            
            if (!entry.loaded) {            
               if (tact::Library::loadSignal(entry.disk, entry.name))
                    entry.loaded = true;
            }
            gui.visualizer.setRenderedSignal(entry.disk, Blues::DeepSkyBlue);
        }
        SignalSource(entry.name, entry.disk);
        if (ImGui::BeginPopupContextItem(entry.name.c_str())){
            if (ImGui::Selectable("Edit")) {
                gui.workspace.designer.edit(entry.disk);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Selectable("Delete")) {
                tact::Library::deleteSignal(entry.name);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::BeginMenu("Export")) {
                static std::string savePath;
                if (ImGui::Selectable("SIG") && mahi::gui::pick_dialog(savePath) == DialogResult::DialogOkay) {
                    fs::path p = savePath + "/" + entry.name + ".sig";
                    tact::Library::exportSignal(getSelectedSignal(), p.generic_string(), tact::FileFormat::SIG);
                    gui.status.pushMessage("Exported Signal " + entry.name + " to " + p.generic_string());
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::Selectable("WAV") && mahi::gui::pick_dialog(savePath) == DialogResult::DialogOkay) {
                    fs::path p = savePath + "/" + entry.name + ".wav";
                    tact::Library::exportSignal(getSelectedSignal(), p.generic_string(), tact::FileFormat::WAV);
                    gui.status.pushMessage("Exported Signal " + entry.name + " to " + p.generic_string());
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::Selectable("CSV") && mahi::gui::pick_dialog(savePath) == DialogResult::DialogOkay) {
                    fs::path p = savePath + "/" + entry.name + ".csv";
                    tact::Library::exportSignal(getSelectedSignal(), p.generic_string(), tact::FileFormat::CSV);
                    gui.status.pushMessage("Exported Signal " + entry.name + " to " + p.generic_string());
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::Selectable("JSON") && mahi::gui::pick_dialog(savePath) == DialogResult::DialogOkay) {
                    fs::path p = savePath + "/" + entry.name + ".json";
                    tact::Library::exportSignal(getSelectedSignal(), p.generic_string(), tact::FileFormat::JSON);
                    gui.status.pushMessage("Exported Signal " + entry.name + " to " + p.generic_string());
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndMenu();
            }
             #ifdef _WIN32
            if (ImGui::Selectable("Reveal in Explorer")) 
            #elif __APPLE__
            if (ImGui::Selectable("Reveal in Finder")) 
            #else
            if (ImGui::Selectable("Reveal in File Browser"))
            #endif
            {
                mahi::gui::open_folder(tact::Library::getLibraryDirectory());
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void Library::renderLibraryControls()
{
    bool disabled = m_lib.count(m_selected) == 0;
    ImGui::BeginDisabled(disabled);

    // SAVE
    float buttonWidth = (ImGui::GetContentRegionAvailWidth() - 5 * ImGui::GetStyle().ItemSpacing.x) / 6;    
    if (ImGui::Button(ICON_FA_SAVE, ImVec2(buttonWidth, 0)) || (m_focused && ImGui::IsKeyPressed(GLFW_KEY_S, false) && ImGui::GetIO().KeyCtrl))
    {
        tact::Signal sig;
        if (gui.workspace.activeTab == Workspace::TabSequencer)
            sig = gui.workspace.sequencer.buildSignal();
        else
            sig = gui.workspace.designer.buildSignal();
        tact::Library::saveSignal(sig, m_selected);
        m_lib[m_selected].disk = sig;
        m_lib[m_selected].loaded = true;
        m_ignoreFilChange = true;
        gui.status.pushMessage("Saved Signal " + m_selected);
    }
    gui.status.showTooltip("Save Selected Signal");

    // DELETE
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_TRASH, ImVec2(buttonWidth, 0)))
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
        gui.status.pushMessage("Deleted Signal " + m_selected);
        m_ignoreFilChange = true;
    }
    gui.status.showTooltip("Delete Selected Signal");


    // EXPORT
    ImGui::SameLine();
    static std::string savePath;
    if (ImGui::Button(ICON_FA_FILE_EXPORT, ImVec2(buttonWidth, 0)) && mahi::gui::pick_dialog(savePath) == DialogResult::DialogOkay)
    {
        fs::path p = savePath + "/" + m_selected + ".sig";
        tact::Library::exportSignal(getSelectedSignal(), p.generic_string(), tact::FileFormat::SIG);
        gui.status.pushMessage("Exported Signal " + m_selected + " to " + p.generic_string());
    }
    gui.status.showTooltip("Export Selected Signal");
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_MUSIC, ImVec2(buttonWidth, 0)) && mahi::gui::pick_dialog(savePath) == DialogResult::DialogOkay)
    {
        fs::path p = savePath + "/" + m_selected + ".wav";
        tact::Library::exportSignal(getSelectedSignal(), p.generic_string(), tact::FileFormat::WAV);
        gui.status.pushMessage("Exported Signal " + m_selected + " to " + p.generic_string());
    }
    gui.status.showTooltip("Export Selected Signal as WAV");
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_TABLE, ImVec2(buttonWidth, 0)) && mahi::gui::pick_dialog(savePath) == DialogResult::DialogOkay)
    {
        fs::path p = savePath + "/" + m_selected + ".csv";
        tact::Library::exportSignal(getSelectedSignal(), p.generic_string(), tact::FileFormat::CSV);
        gui.status.pushMessage("Exported Signal " + m_selected + " to " + p.generic_string());
    }
    gui.status.showTooltip("Export Selected Signal as CSV");

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_CODE, ImVec2(buttonWidth, 0)) && mahi::gui::pick_dialog(savePath) == DialogResult::DialogOkay)
    {
        fs::path p = savePath + "/" + m_selected + ".json";
        tact::Library::exportSignal(getSelectedSignal(), p.generic_string(), tact::FileFormat::JSON);
        gui.status.pushMessage("Exported Signal " + m_selected + " to " + p.generic_string());
    }
    gui.status.showTooltip("Export Selected Signal as JSON");

    ImGui::EndDisabled();
}
