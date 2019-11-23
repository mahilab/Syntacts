#include <tact>
#include <iostream>
#include <thread>
#include <chrono>

using namespace tact;

void sleep(float sec) {
    if (sec != INF)
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec * 1500)));
    else
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(60 * 1500)));
}

int main(int argc, char const *argv[])
{
    // Session session;
    // session.open(8);    
    
    auto signal = Square(440) * 0.25f;

    Library::exportSignal(signal, "myFile", FileFormat::AIFF);  



    return 0;
}
