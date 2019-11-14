#include <Syntacts/Session.hpp>
#include <iostream>

using namespace tact;

int main(int argc, char const *argv[])
{
    Session session;

    for (auto& d : session.getAvailableDevices())
        std::cout << d.first << "," << d.second.name << std::endl;

    auto& devs = session.getAvailableDevices();
    session.open(devs.at(12));

    std::cout << session.getCurrentDevice().name << std::endl;
    std::cout << session.isOpen() << std::endl;


    return 0;
}
