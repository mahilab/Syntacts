#pragma once

#include "Widget.hpp"
#include <deque>

class DeviceBar : public Widget {
public:
    DeviceBar(Gui& gui);
    ~DeviceBar();
    void initialize();
    void switchDevice(const tact::Device& dev, double sampleRate = 0);
    void switchApi(const std::string& api);
    void switchSampleRate(double sampleRate);
    void update() override;
private:
    void renderApiSelection();
    void renderDeviceSelection();
    void renderDeviceSampleRates();
    void renderDeviceDetails();
    void getCurrent();
    void getAvailable();
public:
    mahi::util::Event<void(void)> onSessionOpen;
    mahi::util::Event<void(void)> onSessionDestroy;
    std::shared_ptr<tact::Session> session;
private:
    tact::Device m_currentDev;
    std::string m_currentApi;
    std::map<std::string, std::deque<tact::Device>> m_available;
};