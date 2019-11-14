#include <Syntacts/Syntacts.hpp>
#include <chrono>
#include <thread>

using namespace tact;


int main(int argc, char const *argv[])
{

    Session session;
    session.open();

    auto osc = create<Sine>(440);
    auto cue = create<Cue>(osc, 5);

    session.play(0, cue);
    
    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}
