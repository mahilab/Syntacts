#pragma once

#include <carnot>
#include <syntacts>
#include "InfoBar.hpp"
#include "helpers.hpp"
#include <deque>

using namespace carnot;

class DeviceBar : public GameObject {
public:

    DeviceBar(const Name& name) : GameObject(name) {
        initialize();
    }

    /// Restarts Syntacts session
    void initialize() {
        session = make<tact::Session>();
        int result = session->open();
        if (result != SyntactsError_NoError && m_infoBar)
            return m_infoBar->pushMessage("Failed to open device! Error: " + str(result), InfoBar::Error);
        getCurrent();
        getAvailable();
        onDeviceUpdated.emit();
    }

    void switchDevice(const tact::Device& dev) {
        int result = session->close();
        if (result != SyntactsError_NoError && m_infoBar)
            m_infoBar->pushMessage("Failed to close device! Error: " + str(result), InfoBar::Error);
        result = session->open(dev);
        if (result != SyntactsError_NoError && m_infoBar)
            m_infoBar->pushMessage("Failed to open device! Error: " + str(result), InfoBar::Error);
        getCurrent();
        onDeviceUpdated.emit();
        std::cout << m_currentDev.name << std::endl;
    }

    void switchApi(const std::string& api) {
        if (m_currentApi != api)
        {
            m_currentApi = api;
            switchDevice(m_available[m_currentApi][0]);
            // m_infoBar->pushMessage("Switched API to " + m_currentApi);
        }        
    }

    void switchSampleRate(double sampleRate) {
        int result = session->close();
        if (result != SyntactsError_NoError && m_infoBar)
            m_infoBar->pushMessage("Failed to close device! Error: " + str(result), InfoBar::Error);
        result = session->open(m_currentDev, m_currentDev.maxChannels, sampleRate);
        if (result != SyntactsError_NoError && m_infoBar)
            m_infoBar->pushMessage("Failed to open device! Error: " + str(result), InfoBar::Error);        
        m_infoBar->pushMessage("Changed sample rate to " + str(sampleRate, "Hz"));
        onDeviceUpdated.emit();
    }

private:

    void start() {
        m_infoBar = findSibling<InfoBar>();
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
        updateDeviceSampleRates();
        ImGui::SameLine();
        updateDeviceDetails();
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_SYNC_ALT)) 
            initialize();
        m_infoBar->tooltip("Refresh Device List");
        ImGui::EndGroup();
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_HELP")) {

            }
            ImGui::EndDragDropTarget();
        }
        ImGui::End();
        ImGui::PopStyleVar();
        m_infoBar->cpuLoad = session->getCpuLoad();
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
        ImGui::PopItemWidth();
        m_infoBar->tooltip("Select API");
    }

    void updateDeviceSelection() {
        ImGui::PushItemWidth(300);
        if (ImGui::BeginCombo("##Devices", m_currentDev.name.c_str())) 
        {
            for (int i = 0; i < m_available[m_currentApi].size(); i++)
            {
                bool selected = (m_currentDev.index == m_available[m_currentApi][i].index);
                if (ImGui::Selectable(m_available[m_currentApi][i].name.c_str(), selected))
                    switchDevice(m_available[m_currentApi][i]);
                if (selected)
                    ImGui::SetItemDefaultFocus(); 
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();
        m_infoBar->tooltip("Select Device");
    }

    void updateDeviceSampleRates() {
        ImGui::PushItemWidth(-65);
        std::string rate = str(session->getSampleRate(), "Hz");
        if (ImGui::BeginCombo("##SampleRates", rate.c_str())) {
            for (auto& s : m_currentDev.sampleRates) {
                rate = str(s, "Hz");
                bool selected = session->getSampleRate() == s;
                if (ImGui::Selectable(rate.c_str(), selected)) {
                    switchSampleRate(s);
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
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
            ImGui::Text("ID"); ImGui::SameLine(50);
            ImGui::Text("Name");  ImGui::SameLine(400);
            ImGui::Text("API"); ImGui::SameLine(500);
            ImGui::Text("Channels");
            for (auto& pair : session->getAvailableDevices()) {
                auto d = pair.second;
                auto api = d.apiName;
                if (d.isApiDefault)
                    api += "*";
                std::string id = str(d.index);
                if (d.isDefault)
                    id += "*";
                ImGui::Separator();
                ImGui::Text(id.c_str());   ImGui::SameLine(50);
                ImGui::Text(d.name.c_str()); ImGui::SameLine(400);
                ImGui::Text(api.c_str());  ImGui::SameLine(500);
                ImGui::Text(str(d.maxChannels).c_str());
            }
            ImGui::EndPopup();
        }
    }



    void getCurrent() {
        m_currentDev = session->getCurrentDevice();
        m_currentApi = m_currentDev.apiName;
    }

    void getAvailable() {
        m_available.clear();
        auto devs = session->getAvailableDevices();
        for (auto& dev : devs) {
            if (dev.second.isApiDefault)
                m_available[dev.second.apiName].push_front(dev.second);
            else
                m_available[dev.second.apiName].push_back(dev.second);
        }
    }



public:

    FloatRect rect;
    Signal<void(void)> onDeviceUpdated;
    Ptr<tact::Session> session;

private:

    tact::Device m_currentDev;
    // std::vector<tact::DeviceInfo> m_available;
    std::string m_currentApi;
    std::map<std::string, std::deque<tact::Device>> m_available;
    Handle<InfoBar> m_infoBar;

};