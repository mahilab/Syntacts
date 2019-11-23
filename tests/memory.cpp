#include <syntacts>
#include <iostream>

using namespace tact;

int main(int argc, char const *argv[])
{
    std::cout << sizeof(Signal::Model<Scalar>) << std::endl;
    std::cout << sizeof(Signal::Model<Ramp>) << std::endl;
    std::cout << sizeof(Signal::Model<Noise>) << std::endl;
    std::cout << sizeof(Signal::Model<Sine>) << std::endl;
    std::cout << sizeof(Signal::Model<Square>) << std::endl;
    std::cout << sizeof(Signal::Model<Saw>) << std::endl;
    std::cout << sizeof(Signal::Model<Triangle>) << std::endl;
    std::cout << sizeof(Signal::Model<SineFM>) << std::endl;
    std::cout << sizeof(Signal::Model<Chirp>) << std::endl;
    std::cout << sizeof(Signal::Model<Envelope>) << std::endl;
    std::cout << sizeof(Signal::Model<KeyedEnvelope>) << std::endl;
    std::cout << sizeof(Signal::Model<ASR>) << std::endl;
    std::cout << sizeof(Signal::Model<ADSR>) << std::endl;
    std::cout << sizeof(Signal::Model<SignalEnvelope>) << std::endl;

    return 0;
}
