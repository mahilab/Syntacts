#include <syntacts>
#include <iostream>
#include <fstream>
#include <typeinfo>

using namespace tact;

int main(int argc, char const *argv[])
{  

    Session session;
    session.open();


    session.setVolume(0, 0.55);

    std::cout << session.getVolume(0) << std::endl;

    sleep(2);

    std::cout << "no crash" << std::endl;

    // sleep(35);

}
