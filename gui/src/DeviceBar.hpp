#pragma once

#include <carnot>
#include <Syntacts/Syntacts.hpp>
#include "InfoBar.hpp"
#include "helpers.hpp"
#include <deque>

using namespace carnot;

class DeviceBar : public GameObject {
public:

    using GameObject::GameObject;

     /// Restarts Syntacts and initializes a specifc device
    void initialize(tact::DeviceInfo device) {
        tact::finalize();
        tact::initialize(device.index, device.maxChannels);
        getAvailable();
        getCurrent();
        onInitialize.emit();
    }

private:

    void start() {
        m_infoBar = findSibling<InfoBar>();
        getCurrent();
        getAvailable();
    }

    void update() override {
        helpers::setWindowRect(rect);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(7,7));
        ImGui::Begin(getName().c_str(), nullptr,  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        ImGui::BeginGroup();
        updateApiSelection();
        ImGui::SameLine();
        updateDeviceSelection();
        ImGui::SameLine();
        updateDeviceDetails();
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_SYNC_ALT)) 
            initialize(m_currentDev);
        m_infoBar->tooltip("Refresh Device List");
        ImGui::EndGroup();
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* playload = ImGui::AcceptDragDropPayload("DND_HELP")) {
                    print("HEY!");
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void updateApiSelection() {
        ImGui::PushItemWidth(100);
        if (ImGui::BeginCombo("##APIs", m_currentApi.c_str())) {
            for (auto& pair : m_available) {
                bool isSelected = m_currentApi == pair.first;
                if (ImGui::Selectable(pair.first.c_str(), isSelected))
                {
                    switchApi(pair.first);
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        m_infoBar->tooltip("Select API");
        ImGui::PopItemWidth();
    }

    void updateDeviceSelection() {
        ImGui::PushItemWidth(-65);
        if (ImGui::BeginCombo("##Devices", m_currentDev.name.c_str())) 
        {
            for (int i = 0; i < m_available[m_currentApi].size(); i++)
            {
                bool is_selected = (m_currentDev.index == m_available[m_currentApi][i].index);
                if (ImGui::Selectable(m_available[m_currentApi][i].name.c_str(), is_selected))
                    initialize(m_available[m_currentApi][i]);
                if (is_selected)
                    ImGui::SetItemDefaultFocus(); 
            }
            ImGui::EndCombo();
        }
        m_infoBar->tooltip("Select Device");
        ImGui::PopItemWidth();
    }

    void updateDeviceDetails() {
        if (ImGui::Button("...")) {
            ImGui::OpenPopup("Device Details");
        }

        m_infoBar->tooltip("List Devices");
        bool modalOpen = true;
        if (ImGui::BeginPopupModal("Device Details", &modalOpen, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
        {
            ImGui::Text("ID"); ImGui::SameLine(40);
            ImGui::Text("Name");  ImGui::SameLine(500);
            ImGui::Text("API");             
            for (auto& pair : m_available) {
                for (auto& d : pair.second) {
                    auto api = d.api;
                    if (d.defaultApi)
                        api += "*";
                    std::string id = str(d.index);
                    if (d.default)
                        id += "*";
                    ImGui::Separator();
                    ImGui::Text(id.c_str());   ImGui::SameLine(40);
                    ImGui::Text(d.name.c_str()); ImGui::SameLine(500);
                    ImGui::Text(api.c_str()); 
                }    
            }
            ImGui::EndPopup();
        }
    }

    void getCurrent() {
        m_currentDev = tact::getCurrentDevice();
        m_currentApi = m_currentDev.api;
    }

    void getAvailable() {
        m_available.clear();
        auto devs = tact::getAvailableDevices();
        for (auto& dev : devs) {
            if (dev.defaultApi)
                m_available[dev.api].push_front(dev);
            else
                m_available[dev.api].push_back(dev);
        }
    }

    void switchApi(const std::string& api) {
        if (m_currentApi != api)
        {
            m_currentApi = api;
            initialize(m_available[m_currentApi][0]);
            m_infoBar->pushMessage("Switched API to " + m_currentApi);
        }        
    }

public:

    FloatRect rect;
    Signal<void(void)> onInitialize;

private:

    tact::DeviceInfo m_currentDev;
    // std::vector<tact::DeviceInfo> m_available;
    std::string m_currentApi;
    std::map<std::string, std::deque<tact::DeviceInfo>> m_available;
    Handle<InfoBar> m_infoBar;

};