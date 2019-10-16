#pragma once
#include <memory>
#include <string>

namespace cereal
{
class access;
}

namespace mylib {

class Base
{
public:
    Base();
    Base(int x);
    virtual void go();

protected:
    friend class cereal::access;

    template <class Archive>
    void serialize(Archive &ar) { ar(cereal::make_nvp("x", m_x)); }

    int m_x;
};

class Derived : public Base
{
public:
    Derived();
    Derived(int x, int y);
    virtual void go();

protected:
    friend class cereal::access;

    template <class Archive>
    void serialize(Archive &ar) { ar(cereal::make_nvp("Base", cereal::base_class<Base>(this)), cereal::make_nvp("y", m_y)); }

    int m_y;
};

void save(const std::shared_ptr<Base> &base, const std::string &filename);
void load(std::shared_ptr<Base> &base, const std::string &filename);

}