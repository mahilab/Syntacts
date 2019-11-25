#include <syntacts>
#include <thread>

using namespace tact;

int main(int argc, char const *argv[])
{
    Signal x = Sine(440) * Envelope(5) << Saw(660) * Envelope(6,0.25);
    Library::saveSignal(x, "x");
    Signal y;
    Library::loadSignal(y, "x");
    std::cout << y.length() << std::endl;
    Library::exportSignal(x, "x", FileFormat::JSON);

    return 0;
}
