#include <Syntacts/Syntacts.hpp>
#include <iostream>

using namespace tact;

struct I1 : public Cloneable<Interface<I1>> {
    I1() { std::cout << "I1 created" << std::endl; }
    ~I1() { std::cout << "I1 destroyed" << std::endl; }
    virtual void i1() = 0;
};

struct I2 : public Cloneable<Interface<I2>, I1> {
    I2() { std::cout << "I2 created" << std::endl; }
    ~I2() { std::cout << "I2 destroyed" << std::endl; }
    virtual void i2() = 0;
};

struct C1 : public Cloneable<C1, I2> {
    C1(int _one, int _two) : one(_one), two(_two)
    { std::cout << "C1 created" << std::endl; }
    ~C1() { std::cout << "C1 destroyed" << std::endl; }
    virtual void i1() override { std::cout << one << std::endl; }
    virtual void i2() override { std::cout << two << std::endl; }
    void c1() {
        std::cout << "C1: " << one << ", " << two << std::endl;
    }
    int one;
    int two;
};

struct A1 : public Cloneable<A1> {
    A1() { std::cout << "A1 created" << std::endl; }
    ~A1() { std::cout << "A1 destroyed" << std::endl; }
    int a = 2;
};

struct C2 : public Cloneable<C2, C1> {
    C2() : Cloneable<C2,C1>(3,5)
    { std::cout << "C2 created" << std::endl; }
    ~C2() { std::cout << "C2 destroyed" << std::endl; }
    void c2() {
        std::cout << "C2: " << one << ", " << two << std::endl;
    }
    Ptr<A1> a1;

private:
    Cloneable* clone_impl() const override {
        std::cout << "Cloned C2" << std::endl;
        auto ret = new C2(*this);
        if (ret->a1 != nullptr)
            ret->a1 = ret->a1->clone();
        return ret;
    }
};

int main(int argc, char const *argv[])
{
    auto c2 = create<C2>();
    c2->a1 = create<A1>();

    Ptr<I1> i1 = c2->clone();
    i1->i1();
    Ptr<C1> c1 = downcast<C2>(i1);
    c1->c1();
    return 0;
}
