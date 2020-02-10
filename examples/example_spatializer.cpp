#include <syntacts>

using namespace tact;

int main(int argc, char const *argv[])
{
    Session ss;
    ss.open();

    Signal sig = Noise();

    // Create spatializer
    Spatializer sp(&ss);

    // set up position of channels, evenly distributed 
    int chs = ss.getChannelCount();
    double spc = 1.0 / (chs - 1);
    for (int i = 0; i < ss.getChannelCount(); ++i) 
        sp.setPosition(i, i * spc);

    // set up target where vibration will be played
    sp.setTarget(0.0);
    sp.setRadius(0.5);
    
    // play
    sp.play(sig);

    // make moving target on pre-described path
    double t = 0;
    while (t < 10) {
        double x = 0.5 + 0.5 * std::sin(2*PI*t);
        sp.setTarget(x);
        sleep(0.01);
        t += 0.01;
    }

    std::cout << "Finished!" << std::endl;
    return 0;
}
