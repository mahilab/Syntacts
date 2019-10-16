#include "serial_lib.hpp"

using namespace mylib;

int main(int argc, char const *argv[])
{
    std::shared_ptr<Base> out = std::make_shared<Derived>(45, 33);
    out->go();
    save(out, "base.json");

    std::shared_ptr<Base> in;
    load(in, "base.json");
    in->go();

    return 0;
}
