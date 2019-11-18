#include <Syntacts/Syntacts.hpp>
#include <chrono>
#include <thread>

using namespace tact;

void sleep(float sec) {
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec * 1000)));
}

int main(int argc, char const *argv[])
{

    Session session;
    session.open(27);

    sleep(1);

    auto Dsharp = create<Cue>(create<Saw>(4 * 311.1270f), create<ASR>(0.05f, 0.15f, 0.05f));
    auto Csharp = create<Cue>(create<Saw>(4 * 277.1826f), create<ASR>(0.05f, 0.15f, 0.05f));
    auto Asharp = create<Cue>(create<Saw>(4 * 233.0819f), create<ASR>(0.05f, 0.15f, 0.05f));
    auto Gsharp = create<Cue>(create<Saw>(4 * 415.3047f), create<ASR>(0.05f, 0.15f, 0.05f));
    auto G      = create<Cue>(create<Saw>(4 * 391.9954f), create<ASR>(0.05f, 0.15f, 0.05f));

    std::vector<Ptr<Cue>> notes = {Dsharp, Dsharp, Csharp, Dsharp, Asharp, Asharp, Dsharp, Gsharp, G,     Dsharp};
    std::vector<float>    delay = {0.25f,  0.25f,  0.25f,  0.25,   0.5f,   0.25f,  0.25f,  0.25f,  0.25f, 1.0f  };

    while (true) {
        for (std::size_t i = 0; i < notes.size(); ++i) {
            session.playAll(notes[i]);
            sleep(delay[i]);
        }
    }

    return 0;
}
