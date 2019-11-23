#include <Syntacts/Syntacts.hpp>
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

    auto cue1 = 2 << create<Saw>(660);
    auto cue2 = create<Sine>(440) * create<Envelope>(2) << create<Sine>(880) * create<Envelope>(2);

    auto cue = cue1->clone();

    std::cout << cue2->length() << std::endl;
  
    auto cuecue = create<Cue>();
    cuecue->setEnvelope(cue->length());
    cuecue->chain(cue);

    std::cout << "here" << std::endl;

    session.playAll(cuecue);
    sleep(1.1 * 5);
    

    return 0;
}
