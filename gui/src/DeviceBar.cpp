#include "DeviceBar.hpp"
#include <thread>
#include "Custom.hpp"
#include "Gui.hpp"
#include <fstream>

using namespace mahi::gui;


DeviceBar::DeviceBar(Gui& gui) :
    Widget(gui)
{ 
    initialize();
}

DeviceBar::~DeviceBar() {
    onSessionDestroy.emit();
    std::ofstream file(gui.saveDir() + "device.ini");
    if (file.is_open()) {
        auto d = session->getCurrentDevice();
        file << d.apiName << std::endl;
        file << d.name << std::endl;
        file << session->getSampleRate();
    }
}

/// Restarts Syntacts session
void DeviceBar::initialize()
{
    // make new session
    onSessionDestroy.emit();
    session = std::make_shared<tact::Session>();
    getAvailable();
    // try to get the last known device
    std::ifstream file(gui.saveDir() + "device.ini");
    bool openLast = false;
    if (file.is_open()) {
        std::string apiName, name, sampleRate;
        std::getline( file, apiName );
        if (m_available.count(apiName)) {
            std::getline( file, name );
            for (auto& d : m_available[apiName]) {
                if (d.name == name) {
                    std::getline( file, sampleRate);
                    std::stringstream ss(sampleRate);
                    double sr;
                    ss >> sr;
                    switchDevice(d, sr);
                    openLast = true;
                }
            }
        }
    }
    if (!openLast)
        switchDevice(session->getDefaultDevice());
}


void DeviceBar::switchDevice(const tact::Device &dev, double sampleRate)
{
    int result = SyntactsError_NoError;
    // close if open
    if (session->isOpen())
        result = session->close();
    if (result != SyntactsError_NoError)
    {
        gui.status.pushMessage("Failed to close device! Error: " + std::to_string(result), StatusBar::Error);
        return;
    }
    // open requested device
    result = session->open(dev, dev.maxChannels, sampleRate);
    getCurrent();
    if (result != SyntactsError_NoError)
    {
        gui.status.pushMessage("Failed to open device! Error: " + std::to_string(result), StatusBar::Error);
        return;
    }
    // update current and available
    std::string msg = "Opened \"" + dev.name + "\" under " + dev.apiName + " @ " + std::to_string((int)session->getSampleRate()) + " Hz";
    gui.status.pushMessage(msg, StatusBar::Info);
    onSessionOpen.emit();
}

void DeviceBar::switchApi(const std::string &api)
{
    if (m_currentApi != api && m_available.count(api) > 0)
    {
        m_currentApi = api;
        switchDevice(m_available[m_currentApi][0]);
        // gui.status.pushMessage("Switched API to " + m_currentApi);
    }
}

void DeviceBar::switchSampleRate(double sampleRate)
{
    switchDevice(m_currentDev, sampleRate);
    // gui.status.pushMessage("Changed sample rate to " + str(sampleRate, "Hz"));
}

void DeviceBar::update() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(7, 7));
    ImGui::BeginFixed("Device Bar", position, size, ImGuiWindowFlags_NoTitleBar);

    ImGui::BeginGroup(); // help group
    renderApiSelection();
    ImGui::SameLine();
    renderDeviceSelection();
    ImGui::SameLine();
    renderDeviceSampleRates();
    ImGui::SameLine();
    renderDeviceDetails();
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_SYNC_ALT)) {
        initialize();
        // gui.status.pushMessage("Restarted Session");
    }
    gui.status.showTooltip("Restart Session");
    ImGui::EndGroup();

    if (HelpTarget()) 
        ImGui::OpenPopup("Device Help");
    if (ImGui::BeginHelpPopup("Device Help")) {
        ImGui::BulletText("Use the drop down lists to select your preferred API, Device, and sample rate");
        ImGui::BulletText("Press the " ICON_FA_BARS " button to show a list of all Devices");
        ImGui::BulletText("Press the " ICON_FA_SYNC " button to restart the current Session");
        ImGui::EndPopup();
    }


    ImGui::End();
    ImGui::PopStyleVar();
}



void DeviceBar::renderApiSelection()
{
    ImGui::PushItemWidth(100);
    if (ImGui::BeginCombo("##APIs", m_currentApi.c_str()))
    {
        for (auto &pair : m_available)
        {
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
    gui.status.showTooltip("Select API");
}

void DeviceBar::renderDeviceSelection()
{
    float wrem = ImGui::GetContentRegionAvail().x;
    ImGui::PushItemWidth(wrem - 135);
    static std::string devName;
    devName.clear();
    if (session)
        devName = m_currentDev.name + " [" + std::to_string(m_currentDev.index) + "]";
    if (ImGui::BeginCombo("##Devices", devName.c_str()))
    {
        for (int i = 0; i < m_available[m_currentApi].size(); i++)
        {
            bool selected = (m_currentDev.index == m_available[m_currentApi][i].index);
            if (m_available[m_currentApi][i].index < 10)
                devName = "[" + std::to_string(m_available[m_currentApi][i].index) + "]  " + m_available[m_currentApi][i].name;
            else
                devName = "[" + std::to_string(m_available[m_currentApi][i].index) + "] " + m_available[m_currentApi][i].name;
            if (ImGui::Selectable(devName.c_str(), selected))
                switchDevice(m_available[m_currentApi][i]);
            if (selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
    gui.status.showTooltip("Select Device");
}

void DeviceBar::renderDeviceSampleRates()
{
    ImGui::PushItemWidth(85);
    static std::string rate;
    rate.clear();
    if (session)
        rate = fmt::format("{} Hz", (int)session->getSampleRate());
    if (ImGui::BeginCombo("##SampleRates", rate.c_str()))
    {
        for (auto &s : m_currentDev.sampleRates)
        {
            rate = fmt::format("{} Hz", (int)s);
            bool selected = false;
            if (session)
                selected = session->getSampleRate() == s;
            if (ImGui::Selectable(rate.c_str(), selected))
            {
                switchSampleRate(s);
            }
            if (selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
    gui.status.showTooltip("Select Sample Rate");
}

void DeviceBar::renderDeviceDetails()
{
    if (ImGui::Button(ICON_FA_BARS) && session)
    {
        ImGui::OpenPopup("Device Details");
    }

    gui.status.showTooltip("List Devices");
    bool modalOpen = true;
    if (ImGui::BeginPopupModal("Device Details", &modalOpen, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
    {
        // ImGui::SameLine(1);
        ImGui::Text("ID");
        ImGui::SameLine(50);
        ImGui::Text("Name");
        ImGui::SameLine(400);
        ImGui::Text("API");
        ImGui::SameLine(500);
        ImGui::Text("Channels");
        ImGui::Separator();
        ImGui::PushStyleColor(ImGuiCol_Button, {0,0,0,0});
        for (auto &pair : session->getAvailableDevices())
        {
            auto d = pair.second;
            auto api = d.apiName;
            if (d.isApiDefault)
                api += "*";
            std::string id = std::to_string(d.index);
            if (d.isDefault)
                id += "*";
            // ImGui::Separator();
            ImGui::PushID(d.index);
            if (ImGui::Button("##", ImVec2(-1, 0)))
            {
                switchDevice(d);
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopID();
            ImGui::SameLine(9);
            ImGui::Text(id.c_str());
            ImGui::SameLine(50);
            ImGui::Text(d.name.c_str());
            ImGui::SameLine(400);
            ImGui::Text(api.c_str());
            ImGui::SameLine(500);
            ImGui::Text(std::to_string(d.maxChannels).c_str());
        }
        ImGui::PopStyleColor();
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
        ImGui::Separator();
        ImGui::Text("*Indicates system default device or API default device");
        ImGui::PopStyleVar();
        ImGui::EndPopup();
    }
}

void DeviceBar::getCurrent()
{
    if (session) {
        m_currentDev = session->getCurrentDevice();
        if (m_currentDev.api != tact::API::Unknown)
            m_currentApi = m_currentDev.apiName;
    }
}

void DeviceBar::getAvailable()
{
    m_available.clear();
    if (session) {
        auto devs = session->getAvailableDevices();
        for (auto &dev : devs)
        {
            if (dev.second.api != tact::API::Unknown && dev.second.index != -1 && dev.second.apiName != "N/A") {
                if (dev.second.isApiDefault)
                    m_available[dev.second.apiName].push_front(dev.second);
                else
                    m_available[dev.second.apiName].push_back(dev.second);
            }
        }
    }
}