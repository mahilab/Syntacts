#include <Tact/Syntacts.hpp>
#include <chrono>
#include <thread>
#include <iostream>

using namespace tact;

void sleep(float sec) {
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec * 1000)));
}

int main(int argc, char const *argv[])
{

    Session session;
    session.open();

    sleep(0.5);

    auto env = create<ASR>(0.05f, 0.1f, 0.04f);

    auto Dsharp = create<Sine>(4 * 311.1270f) * env;
    auto Csharp = create<Sine>(4 * 277.1826f) * env;
    auto Asharp = create<Sine>(4 * 233.0819f) * env;
    auto Gsharp = create<Sine>(4 * 415.3047f) * env;
    auto G      = create<Sine>(4 * 391.9954f) * env;    

    // std::vector<Ptr<Cue>> notes = {Dsharp, Dsharp, Csharp, Dsharp, Asharp, Asharp, Dsharp, Gsharp, G,     Dsharp};
    // std::vector<float>    delay = {0.25f,  0.25f,  0.25f,  0.5f,   0.5f,   0.25f,  0.25f,  0.25f,  0.25f, 1.0f  };

    auto seq = create<Sequence>();
    seq->insert(0.00f, Dsharp);
    seq->insert(0.25f, Dsharp);
    seq->insert(0.50f, Csharp);
    seq->insert(0.75f, Dsharp);
    seq->insert(1.25f, Asharp);
    seq->insert(1.75f, Asharp);
    seq->insert(2.00f, Dsharp);
    seq->insert(2.25f, Gsharp);
    seq->insert(2.50f, G     );
    seq->insert(2.75f, Dsharp);

    std::cout << seq->length() << std::endl;

    auto cue = create<Cue>();
    cue->setEnvelope(seq->length());
    cue->chain(seq);


    session.playAll(cue);
    sleep(4);

    // while (true) {
    //     // for (std::size_t i = 0; i < notes.size(); ++i) {
    //     //     session.playAll(notes[i]);
    //     //     sleep(delay[i]);
    //     // }
    // }

    return 0;
}
