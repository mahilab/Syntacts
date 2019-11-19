#include <Syntacts/Syntacts.hpp>
#include <iostream>

using namespace tact;


int main(int argc, char const *argv[])
{

    auto a = create<Scalar>(5);
    auto b = a->clone();

    a->value = 10;

    std::cout << a->sample(0) << std::endl;
    std::cout << b->sample(0) << std::endl;

    return 0;
}
