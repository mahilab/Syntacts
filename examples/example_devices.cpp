#include <syntacts>
#include <iostream>

using namespace tact;

// This example will load all devices and display their information for your computer. 
// Run the exe from a window that will stay open once it runs so you can see the results

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
