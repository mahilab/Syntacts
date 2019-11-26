#include <syntacts>
#include <iostream>

#include <typeinfo>

using namespace tact;

int main(int argc, char const *argv[])
{    
    Signal x = Sine(440);
    Signal y = x;
    std::cout << x.get() << std::endl;
    std::cout << y.get() << std::endl;
    std::cout << Signal::pool().blocksUsed() << std::endl;
    return 0;
}
