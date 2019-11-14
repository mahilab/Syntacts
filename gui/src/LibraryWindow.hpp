#pragma once

#include <Syntacts/Syntacts.hpp>
#include <carnot>
#include <filesystem>
#include "helpers.hpp"
#include "InfoBar.hpp"
#include "DesignerWindow.hpp"
#include <set>

using namespace carnot;
namespace fs = std::filesystem;

class LibraryWindow : public GameObject {
public:

    using GameObject::GameObject;

    struct Entry {
        std::string name;
        fs::path path;
        tact::Ptr<tact::Cue> disk;
        bool saved;
    };

    tact::Ptr<tact::Cue> getSelectedCue() {
        if (m_lib.count(m_selected)) {
            return m_lib[m_selected].disk;
        }   
        return nullptr;
    }

private:

    void start() {
        fs::create_directories(tact::Library::getLibraryDirectory());
        m_infoBar =  findSibling<InfoBar>();
        m_designer = findSibling<DesignerWindow>();
        sync();
        if (m_lib.size() > 0)
            m_selected = m_lib.begin()->first;
        startCoroutine(periodicSync());
    }

    Enumerator periodicSync() {
        while (true) {
            co_yield new WaitForSeconds(1.0f);
            sync();
        }
    }

    void update() override {
        helpers::setWindowRect(rect);
        ImGui::Begin(getName().c_str(), nullptr,  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        ImGui::BeginGroup();
        updateCreateCueDialog();
        ImGui::Separator();
        updateLibraryList();     
        ImGui::Separator();   
        updateListControls();
        ImGui::EndGroup();
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* playload = ImGui::AcceptDragDropPayload("DND_HELP")) {
                    print("HEY!");
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::End();
    }

    void updateCreateCueDialog() {
        ImGui::PushItemWidth(-30);
        bool entered = ImGui::InputText("##CueName", m_inputBuffer, 64, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::SameLine(); 
        std::string filename(m_inputBuffer);
        bool valid = filename != "";

        helpers::beginDisabled(!valid);
        if (ImGui::Button(ICON_FA_PLUS_SQUARE) || (entered && valid)) {
            if (!m_lib.count(filename)) {
                auto cue = m_designer->buildCue();
                tact::Library::saveCue(cue, filename);
                sync();
                m_infoBar->pushMessage("Created Cue " + filename);
                memset(m_inputBuffer, 0, 64);
                m_selected = filename;
            }
            else {
                m_infoBar->pushMessage("Cue " + filename + " already exists", InfoBar::Error);
            }
        }
        m_infoBar->tooltip("Create new Cue");
        helpers::endDisabled(!valid);
    }

    void updateLibraryList() {
        auto avail = ImGui::GetContentRegionAvail();
        ImGui::PushStyleColor(ImGuiCol_ChildBg, Color::Transparent);
        ImGui::BeginChild("CueList", ImVec2(0, avail.y - 29));

        for (auto& pair : m_lib) {
            Entry& entry = pair.second;
            if (ImGui::Selectable(entry.name.c_str(), m_selected == entry.name))
                m_selected = entry.name;           
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_AcceptNoDrawDefaultRect)) {
                ImGui::Text(entry.name.c_str());
                ImGui::EndDragDropSource();
            } 
        }

        ImGui::EndChild();
        ImGui::PopStyleColor();
    }

    void updateListControls() {
        bool disabled = m_lib.count(m_selected) == 0;
        helpers::beginDisabled(disabled);
        if (ImGui::Button(ICON_FA_SAVE)) {
            auto cue = m_designer->buildCue();
            tact::Library::saveCue(cue, m_selected);
            m_lib[m_selected].disk = tact::Library::loadCue(m_selected);
            m_infoBar->pushMessage("Saved Cue " + m_selected);
            sync();
        }
        m_infoBar->tooltip("Save Selected Cue");

        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_TRASH))
        {
            m_infoBar->pushMessage("Deleted Cue " + m_selected);
            auto it = m_lib.find(m_selected);
            auto next = std::next(it);
            auto prev = std::prev(it);
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
        m_infoBar->tooltip("Delete Selected Cue");

        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_FILE_AUDIO)) {
            std::string filePath = m_selected + ".wav";
            tact::Library::exportCue(getSelectedCue(), filePath, tact::ExportFormat::WAV);
            m_infoBar->pushMessage("Exported Cue " + m_selected + " to " + filePath);
        }
        m_infoBar->tooltip("Export Selected Cue to WAV");

        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_FILE_CSV)) {
            std::string filePath = m_selected + ".csv";
            tact::Library::exportCue(getSelectedCue(), filePath, tact::ExportFormat::CSV);
            m_infoBar->pushMessage("Exported Cue " + m_selected + " to " + filePath);
        }
        m_infoBar->tooltip("Export Selected Cue to CSV");

        helpers::endDisabled(disabled);
    }



    void sync() {
        // load new entries from disk
        std::set<std::string> diskNames;
        for (const auto & dir_entry : fs::directory_iterator(tact::Library::getLibraryDirectory()))
        {
            if (dir_entry.path().has_extension() && dir_entry.path().extension() == ".tact") {
                std::string name = dir_entry.path().stem().generic_string();
                diskNames.insert(name);
                if (m_lib.count(name) == 0) {
                    m_lib[name] = Entry();
                    m_lib[name].name = name;
                    m_lib[name].path = dir_entry.path();
                    m_lib[name].disk = tact::Library::loadCue(name);
                    m_lib[name].saved = true;
                }
            }
        }
        // remove stale entries from memory
        for (auto it = m_lib.cbegin(); it != m_lib.cend(); /**/) {
            if (diskNames.count(it->second.name) == 0) 
                m_lib.erase(it++);
            else
                it++;
        }
    }

public:

    FloatRect rect;

private:

    std::string m_selected;
    int m_selection = -1;
    char m_inputBuffer[64] = "";

    std::unordered_map<std::string, Entry> m_lib;

    Handle<InfoBar> m_infoBar;
    Handle<DesignerWindow> m_designer;
};