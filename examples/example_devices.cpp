#include <Syntacts/Session.hpp>
#include <iostream>

int main(int argc, char const *argv[])
{
    tact::Session session;

    for (auto& d : session.getAvailableDevices())
        std::cout << d.first << "," << d.second.name << std::endl;

    auto& devs = session.getAvailableDevices();
    session.open(devs.at(12));

    std::cout << session.getCurrentDevice().name << std::endl;
    std::cout << session.isOpen() << std::endl;


    return 0;
}
