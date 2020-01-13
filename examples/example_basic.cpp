#include <syntacts>
#include <iostream>
#include <fstream>
#include <typeinfo>

using namespace tact;

int main(int argc, char const *argv[])
{  

    Session session;
    session.open();

    Signal s = Sine(440, Sine(10, 20)) * ASR(0.5,0.5,0.5);

    session.play(0, s);

    sleep(2);


    // sleep(35);

}
