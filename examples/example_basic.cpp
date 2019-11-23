#include <tact>
#include <iostream>
#include <thread>
#include <chrono>

using namespace tact;

void sleep(float sec) {
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec * 1500)));
}

int main(int argc, char const *argv[])
{

    Session session;
    session.open(8);    
    
    auto signal = 2 << Saw(440) + 0.5f * ASR(1,3,1) << -2 << Saw(500) * ADSR(1,1,1,1);
    

    session.playAll(signal);
    sleep(signal.length());



    return 0;
}
