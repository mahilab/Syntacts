#include <syntacts>
#include <iostream>
#include <fstream>
#include <typeinfo>

using namespace tact;

int main(int argc, char const *argv[])
{  

    Session session;
    session.open();
    Signal sineFm = Sine(250, Sine(10), 10) * ASR(.1,.3,.1);
    Library::saveSignal(sineFm, "sineFM");
    session.playAll(sineFm);
    sleep(sineFm.length()+1);
    return 0;

}
