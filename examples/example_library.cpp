#include <syntacts>
#include <thread>

using namespace tact;

int main(int argc, char const *argv[])
{

    // std::cout << Signal::pool().blocksUsed() << std::endl;
    // {
    //     Signal x;    
    //     std::cout << Signal::pool().blocksUsed() << std::endl;
    //     Library::loadSignal(x,"x");
    //     std::cout << Signal::pool().blocksUsed() << std::endl;
    // }
    // std::cout << Signal::pool().blocksUsed() << std::endl;
    
    {
        Signal x = Sine(440) * ASR(1,3,1);
        Library::saveSignal(x,"x");
        Signal y;
        Library::loadSignal(y, "x");
    }
    std::cout << Signal::pool().blocksUsed();


    return 0;
}
