#include <syntacts> // include Syntacts in its entirety

using namespace tact; // everything in Syntacts is under the namespace tact

int main(int argc, char const *argv[])
{
    Session session;
    session.open();
    Signal x = Saw(440, 100) * Sine(10);
    Signal r = Ramp(0.1, 0.99, 10);
    Signal f = Filter(Filter::LowPass, x, r, r);
    f = f * ASR(1,8,1);
    session.playAll(f);
    sleep(f.length());
    session.stopAll();
    return 0;
}
