#include <syntacts>
#include <iostream>
#include <fstream>
#include <typeinfo>

using namespace tact;

int main(int argc, char const *argv[])
{  

    Session session;
    session.open();

    Signal x = Sine(440) * Envelope(10);

    // session.play(0, x);
    Library::exportSignal(x, "evan.wav", FileFormat::WAV);

    sleep(35);

}
