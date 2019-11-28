#include <syntacts>
#include <iostream>

#include <typeinfo>

using namespace tact;

int main(int argc, char const *argv[])
{  
    std::cout << sizeof(Signal::Model<Sum>) << std::endl;

    Signal x = Scalar(5);
    std::cout << x.get() << std::endl;
    auto locX = (size_t)x.get();
    std::cout << locX << std::endl;

    Signal y = x;
    std::cout << y.get() << std::endl;
    auto locY = (size_t)y.get();
    std::cout << locY << std::endl;

    std::cout << (locX - locY) << std::endl;

    std::cout << "x: " << x.sample(0) << std::endl;
    std::cout << "y: " << y.sample(0) << std::endl;
    return 0;
}
