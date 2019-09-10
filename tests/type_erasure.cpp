#include <iostream>
#include <vector>



using namespace std;

struct SpecOne
{
    struct Concept
    { // abstract base class for model.
        virtual ~Concept() = default;
        // Your (internal) interface goes here.
        virtual int methodOne() const = 0;
    };
    template <class Holder>
    struct Model : public Holder, public virtual Concept
    {
        using Holder::Holder;
        // Pass through to encapsulated value.
        virtual int methodOne() const override
        {
            return rubber_types::model_get(this).methodOne();
        }
    };
    template <class Container>
    struct ExternalInterface : public Container
    {
        using Container::Container;
        // Define the external interface. Should match encapsulated type.
        int methodOne() const
        {
            return rubber_types::interface_get(this).methodOne();
        }
    };
};

struct SpecTwo
{
    struct Concept
    { // abstract base class for model.
        virtual ~Concept() = default;
        // Your (internal) interface goes here.
        virtual int methodTwo() const = 0;
    };
    template <class Holder>
    struct Model : public Holder, public virtual Concept
    {
        using Holder::Holder;
        // Pass through to encapsulated value.
        virtual int methodTwo() const override
        {
            return rubber_types::model_get(this).methodTwo();
        }
    };
    template <class Container>
    struct ExternalInterface : public Container
    {
        using Container::Container;
        // Define the external interface. Should match encapsulated type.
        int methodTwo() const
        {
            return rubber_types::interface_get(this).methodTwo();
        }
    };
};

using TEClassOne = rubber_types::TypeErasure<SpecOne>;
using TEClassTwo = rubber_types::TypeErasure<SpecTwo>;
using TEClassOneTwo = rubber_types::TypeErasure<SpecOne, SpecTwo>;

struct SomeStruct
{
    int One;
    int Two;
    SomeStruct(int one, int two) : One(one), Two(two)
    {
    }
    int methodOne() const
    {
        std::cout << this->One << std::endl;
        return One;
    }
    int methodTwo() const
    {
        std::cout << this->Two << std::endl;
        return Two;
    }
};

struct AnotherStruct
{
    int methodOne() const {
        return 0;
    }
};

int main()
{

    TEClassOne a = SomeStruct(1, 2);
    a.methodOne();

    TEClassTwo b = SomeStruct(1, 2);
    b.methodTwo();

    TEClassOneTwo c = SomeStruct(1, 2);

    c.methodOne();
    c.methodTwo();

    SomeStruct d(1, 2);

    AnotherStruct e;

    std::vector<TEClassOneTwo> vec;
    vec.emplace_back(SomeStruct(1, 2));
    vec.emplace_back(c); //// c and vec[1] are now the same object!!!!
    vec.emplace_back(d); 
    vec.emplace_back(e);

    a = vec[1]; //// a is now a copy of vec[1]/c

    a.methodOne();

    cin.get();
}
