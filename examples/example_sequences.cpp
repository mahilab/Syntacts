// #define TACT_USE_MALLOC
#include <syntacts>
#include <chrono>
#include <thread>
#include <iostream>

using namespace tact;

void sleep(float sec) {
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(sec * 1000)));
}

int main(int argc, char const *argv[])
{   
    Session s;
    s.open();

    // Sequences allow to you order multiple Signals in time.

    // Most simply, you can concatenate two or more signals:
    Signal sigA = Sine(440) * ASR(1,1,1);
    Signal sigB = Square(440) * ADSR(1,1,1,1);

    Sequence seq1 = sigA << sigB;

    std::cout << sigA.length() << std::endl; // 3 s
    std::cout << sigB.length() << std::endl; // 4 s
    std::cout << seq1.length() << std::endl; // 7 s

    s.playAll(seq1);
    sleep(seq1.length());

    // You can also add delay, pause, and fade with scalar values:
    Sequence seq2 = 1 << sigA << 2 << sigB << -1 << sigA; // 1 s delay, 2 s pause, 1 s fade

    std::cout << seq2.length() << std::endl; // 12 s

    s.playAll(seq2);
    sleep(seq2.length());

    // Of course, Sequences of Sequences is possible:
    Sequence seq3 = seq1 << seq2; // note this will also modify seq1
    std::cout << seq3.length() << std::endl; // 19, we won't play this one :)

    // The << operator inserts a Signal at the Sequence head position, 
    // and then moves it forward or backward. You can get/set the head position
    // manually for the next << operation:

    std::cout << seq2.head << std::endl; // 12
    seq2.head = 2; // set back to 2
    seq2 << Noise() * Envelope(1); // 1 s of noise starts at the 2 second mark

    // You can accomplish the same thing with the insert function
    seq2.insert(Noise() * Envelope(1), 4); // 1 s of noise starts at the 4 second mark

    s.playAll(seq2);
    sleep(seq2.length()); // still 12 s long because the inserted noise didn't extend its length

    return 0;
}
