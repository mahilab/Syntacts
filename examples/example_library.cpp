#include <syntacts>
#include <thread>
#include <iomanip>

using namespace tact;

int main(int argc, char const *argv[])
{

    Session session;
    session.open();

    Signal x = Sine(440) * ASR(1,1,1);

    std::cout << std::boolalpha << Library::exportSignal(x, "signals/xy.wav", FileFormat::WAV, 44100) << std::endl;

    Signal y;

    std::cout << Library::importSignal(y, "signals/bensound-summer.wav") << std::endl;

    Library::saveSignal(y, "loaded");

    Signal z;
    Library::loadSignal(z, "loaded");

    session.play(0, z);

    sleep(z.length());


    return 0;
}
