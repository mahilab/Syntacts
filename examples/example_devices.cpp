#include <syntacts>
#include <iostream>

using namespace tact;

int main(int argc, char const *argv[])
{
    Session session;

    for (auto& d : session.getAvailableDevices())
    {
        auto& dev = d.second;
        std::cout << std::endl;
        std::cout << "Index:        " << dev.index << std::endl;
        std::cout << "Name:         " << dev.name  << std::endl;
        std::cout << "Default:      " << (dev.isDefault ? "Yes" : "No") << std::endl;
        std::cout << "API:          " << dev.apiName << std::endl;
        std::cout << "API Default:  " << (dev.isApiDefault ? "Yes" : "No") << std::endl;
        std::cout << "Max Channels: " << dev.maxChannels << std::endl;
        std::cout << "Sample Rates: ";
                                        for (auto& s : dev.sampleRates)
                                            std::cout << s << " ";
                                        std::cout << std::endl;
    }
    std::cout << std::endl;


    return 0;
}
