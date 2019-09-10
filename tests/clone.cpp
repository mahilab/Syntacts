#include <memory>
#include <iostream>

class Interface {
public:
    virtual ~Interface() = default;
    virtual std::shared_ptr<Interface> clone() const = 0;
    virtual void doSomething() = 0;
};

class Impl1 : public Interface {
public:

    virtual std::shared_ptr<Interface> clone() const override {
        return std::make_shared<Impl1>(*this);
    }

    virtual void doSomething() override {
        std::cout << x++ << std::endl;
    }
    int x = 0;
};

template <typename Derived>
class InterfaceCloneable : public Interface {
public:
    virtual std::shared_ptr<Interface> clone() const override {
        return std::make_shared<Derived>(static_cast<Derived const &>(*this));
    }
};

class Impl2 : public InterfaceCloneable<Impl2> {
public:
    virtual void doSomething() override {
        std::cout << y++ << std::endl;
    }
    virtual std::shared_ptr<Interface> clone() const override {
        std::cout << "Made a clone!" << std::endl;
        return InterfaceCloneable<Impl2>::clone();
    }
    int y = 0;
};

int main(int argc, char const *argv[])
{
    // auto impl1 = std::make_shared<Impl1>();
    // impl1->doSomething();
    // impl1->doSomething();
    // auto impl1Clone = impl1->clone();
    // impl1->doSomething();
    // impl1->doSomething();
    // impl1Clone->doSomething();
    // impl1->doSomething();

    auto impl2 = std::make_shared<Impl2>();
    impl2->doSomething();
    impl2->doSomething();
    auto clone = impl2->clone();
    impl2->doSomething();
    impl2->doSomething();
    clone->doSomething();
    return 0;
}
