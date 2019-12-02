#pragma once

#include <carnot>
#include <syntacts>
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
        tact::Signal disk;
        bool saved;
    };

    tact::Signal getSelectedCue() {
        if (m_lib.count(m_selected)) {
            return m_lib[m_selected].disk;
        }   
        return tact::Signal();
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
        helpers::signalHeld = false;
        helpers::setWindowRect(rect);
        ImGui::Begin(getName().c_str(), nullptr,  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        if (ImGui::BeginTabBar("LibraryWindowTabs")) {
            if (ImGui::BeginTabItem("Pallette ##Tab")) {
                updateSignalList();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Library ##Tab")) {
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
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }

    void updateCreateCueDialog() {
        ImGui::PushItemWidth(-30);
        bool entered = ImGui::InputText("##CueName", m_inputBuffer, 64, ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::PopItemWidth();
        ImGui::SameLine(); 
        std::string filename(m_inputBuffer);
        bool valid = filename != "";

        helpers::beginDisabled(!valid);
        if (ImGui::Button(ICON_FA_PLUS_SQUARE) || (entered && valid)) {
            if (!m_lib.count(filename)) {
                auto cue = m_designer->buildSignal();
                tact::Library::saveSignal(cue, filename);
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

    std::type_index m_payload = typeid(tact::Scalar(0));

    void updateSignalList() {
        static std::vector<std::pair<std::string,std::vector<std::type_index>>> signals = {
            {"Oscillators", {typeid(tact::Sine), typeid(tact::Square), typeid(tact::Saw), typeid(tact::Triangle), typeid(tact::Chirp), typeid(tact::Noise) }},
            {"Envelopes", {typeid(tact::Envelope), typeid(tact::ASR), typeid(tact::ADSR)}},
            {"General", {typeid(tact::Time), typeid(tact::Scalar), typeid(tact::Ramp), typeid(tact::Expression) }},
            {"Processes", {typeid(tact::Sum), typeid(tact::Product) }}
        };
        for (auto& section : signals) {
            // if (ImGui::TreeNode(section.first.c_str())) {
                if (!ImGui::CollapsingHeader(section.first.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                    continue;
                // ImGui::Selectable(section.first.c_str(), true);
                for (auto& sig : section.second) {                
                    auto s = helpers::signalName(sig);
                    ImGui::Selectable(s.c_str(), false);
                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                        m_payload = sig;
                        helpers::signalHeld = true;
                        ImGui::SetDragDropPayload("DND_SIGNAL", &m_payload, sizeof(std::type_index));
                        // ImGui::PushStyleColor(ImGuiCol_Text, Oranges::Orange);
                        ImGui::Text(s.c_str());
                        // ImGui::PopStyleColor();
                        ImGui::EndDragDropSource();
                    } 
                }
                // ImGui::TreePop();
            // }
            // ImGui::Separator();
        }
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
            auto cue = m_designer->buildSignal();
            tact::Library::saveSignal(cue, m_selected);
            tact::Library::loadSignal(m_lib[m_selected].disk, m_selected);
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
        if (ImGui::Button(ICON_FA_MUSIC)) {
            std::string filePath = m_selected;
            tact::Library::exportSignal(getSelectedCue(), filePath, tact::FileFormat::WAV);
            m_infoBar->pushMessage("Exported Cue " + m_selected + " to " + filePath);
        }
        m_infoBar->tooltip("Export Selected Cue to WAV");

        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_TABLE)) {
            std::string filePath = m_selected;
            tact::Library::exportSignal(getSelectedCue(), filePath, tact::FileFormat::CSV);
            m_infoBar->pushMessage("Exported Cue " + m_selected + " to " + filePath);
        }
        m_infoBar->tooltip("Export Selected Cue to CSV");

        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_FILE_EXPORT)) {
        //     std::string filePath = m_selected;
        //     tact::Library::exportSignal(getSelectedCue(), filePath, tact::FileFormat::JSON);
        //     m_infoBar->pushMessage("Exported Cue " + m_selected + " to " + filePath);
        // }
        // m_infoBar->tooltip("Export Selected Cue to JSON");

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
                    tact::Library::loadSignal(m_lib[name].disk, name);
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