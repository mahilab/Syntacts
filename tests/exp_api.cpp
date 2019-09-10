#include <iostream>
#include <cmath>
#include <memory>

class Generator {
public:
    virtual ~Generator() = default;
    virtual float sample(float t) = 0;
};

class Operator : public Generator {
public:
    Operator(std::shared_ptr<Generator> lhs, std::shared_ptr<Generator> rhs) : 
        m_lhs(lhs), m_rhs(rhs) 
    { 

    }

    float sample(float t) {
        return operate(m_lhs->sample(t), m_rhs->sample(t));
    }

    virtual float operate(float lhs, float rhs) = 0;
    
private:
    std::shared_ptr<Generator> m_lhs;
    std::shared_ptr<Generator> m_rhs;
};

class Product : public Operator {
public:
    using Operator::Operator;
    ~Product() {std::cout << "Product died" << std::endl; }
    virtual float operate(float lhs, float rhs) {
        return lhs * rhs;
    }
};


template <typename T>
class Gen {
public:    
    template <typename ...Args>
    Gen(Args... args) {
        ptr = std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <typename OtherGen>
    Gen<Product> multiply(const OtherGen& rhs) {
        return Gen<Product>(ptr, rhs.ptr);
    }

    template <typename OtherGen>
    Gen<Product> operator*(const OtherGen& rhs) {
        return Gen<Product>(ptr, rhs.ptr);
    }

    std::shared_ptr<T> ptr;
};

class Sin : public Generator {
public:
    Sin(float frequency) : frequency(frequency) { }
    ~Sin() { std::cout << frequency << std::endl; }
    float sample(float t) {
        return std::sin(2 * 3.14f * frequency * t);
    }
    float frequency;
};

int main(int argc, char const *argv[])
{
    auto prod1 = Gen<Sin>(175.0f) * Gen<Sin>(160.0f);
    auto x =  Gen<Sin>(200.0f);
    {
        auto prod2 = prod1 *x;
    }
    std::cout << ("meow") << std::endl;
}
