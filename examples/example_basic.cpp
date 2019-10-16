#include <Syntacts/Syntacts.hpp>

using namespace tact;

int main(int argc, char const *argv[])
{
    auto ramp = std::make_shared<Ramp>(175, 250, 3);
    auto sine = std::make_shared<SineWave>(ramp);
    auto envl = std::make_shared<Envelope>(2.0f);
    auto cue  = std::make_shared<Cue>(sine, envl);
    save(cue, "myCue.cue");
    // exportToWave(cue, "myCue.wav");
    return 0;
}
