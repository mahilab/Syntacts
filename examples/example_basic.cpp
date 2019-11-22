#include <Syntacts/Syntacts.hpp>
#include <chrono>
#include <thread>

using namespace tact;


int main(int argc, char const *argv[])
{
    Session session;
    auto def = session.getDefaultDevice();
    session.open(8);

    auto cue1 = create<Cue>();
    cue1->chain(create<Sine>(880));
    cue1->chain(create<Sine>(10));
    cue1->setEnvelope(create<ASR>(1,3,1));

    auto s = (0.5f * create<Saw>(400) + 0.25f * create<Sine>(75)) * create<Sine>(10) * create<ASR>(1,3,1);

    auto cue = create<Cue>();
    cue->setEnvelope(5.0f);
    cue->chain(s);

    Library::exportCue(cue, "wtf.wav");
    Library::saveCue(cue, "wtf", SaveFormat::Binary);

    auto cueLoaded = Library::loadCue("wtf");
    auto cueLoadedCloned = cueLoaded->clone();

    session.playAll(cueLoaded);

    std::this_thread::sleep_for(std::chrono::seconds(6));

    return 0;
}
