#include <syntacts>
#include <iostream>

#include <typeinfo>

using namespace tact;

int main(int argc, char const *argv[])
{  
    auto s = std::make_shared<Session>();
    std::cout << s->open() << std::endl;
    sleep(1);
    std::cout << s->close() << std::endl;
    std::cout << s->open(24) << std::endl;

    Signal x = 5 * Scalar(1) + 1;

    return 0;

}
