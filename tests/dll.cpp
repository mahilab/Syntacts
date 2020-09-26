#include "syntacts.h"
#include <iostream>
#include <thread>
#include <chrono>

void sleep(double seconds) {
    std::this_thread::sleep_for(std::chrono::nanoseconds((std::size_t)(seconds * 1000000000)));
}


int main(int argc, char const *argv[])
{
    auto s = Session_create();
    Session_open1(s);

    std::cout << Debug_sigMapSize() << std::endl;
    std::cout << Signal_count() << std::endl;

    auto sine1 = Sine_create2(440);
    auto sine2 = Sine_create2(10);
    auto adsr1 = ADSR_create(1,1,1,1,1,0.5);
    auto prod1 = Product_create(sine1, sine2);
    auto prod2 = Product_create(prod1, adsr1);

    std::cout << Debug_sigMapSize() << std::endl;
    std::cout << Signal_count() << std::endl;

    Signal_delete(sine1);
    Signal_delete(sine2);
    Signal_delete(adsr1);
    Signal_delete(prod1);

    std::cout << Debug_sigMapSize() << std::endl;
    std::cout << Signal_count() << std::endl;

    Session_play(s, 0, prod2);
    sleep(6);

    Signal_delete(prod2);

    std::cout << Debug_sigMapSize() << std::endl;
    std::cout << Signal_count() << std::endl;

    auto l = Library_loadSignal("funky");
    Session_play(s, 0, l);
    sleep(Signal_length(l));

    Session_delete(s);

    std::cout << Debug_sigMapSize() << std::endl;
    std::cout << Signal_count() << std::endl;

    std::cout << "The End" << std::endl;

    return 0;
}
