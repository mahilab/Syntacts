// #include <Syntacts/Syntacts.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <filesystem>

#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/access.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/functional.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/atomic.hpp>
#include <cereal/access.hpp>

#define TACT_SERIALIZE(...) friend class cereal::access; \
                            template <class Archive> \
                            void serialize(Archive& archive) { \
                                archive(__VA_ARGS__); \
                            }
                            
#define TACT_PARENT(T) ::cereal::make_nvp(#T, cereal::base_class<T>(this))
#define TACT_MEMBER(T) ::cereal::make_nvp(#T, T)

using namespace std;

struct Scalar {
    Scalar() : value(0) {};
    Scalar(float _value) : value(_value) { }
    float sample(float t) const {
        return value;
    }
    float value;
};

class Signal {
public:

    Signal() : Signal(Scalar()) { }

    template<typename T>
    Signal(T signal) : 
        m_ptr(make_shared<Model<T>>(move(signal)))
    {}

    float sample(float t) const {
        return m_ptr->sample(t); 
    }

public:

    struct Concept {
        Concept() = default;
        virtual ~Concept() = default;
        virtual float sample(float t) const = 0;
        template <class Archive>
        void serialize(Archive& archive) {}
    };

    template <typename T>
    struct Model final : Concept {
        Model() = default;
        Model(T model) : m_model(move(model)) { }
        float sample(float t) const override
        {
            return m_model.sample(t);
        }
        T m_model;
        TACT_SERIALIZE(TACT_PARENT(Concept), TACT_MEMBER(m_model));
    };
private:
    shared_ptr<const Concept> m_ptr;
    TACT_SERIALIZE(TACT_MEMBER(m_ptr));
};

struct Sine {
    Sine() : f(1) { }
    Sine(float _f) : f(_f) { }
    float sample(float t) const {
        return sin(6.28f*f*t);
    }
    float f;
    TACT_SERIALIZE(TACT_MEMBER(f));
};

struct Sum {
    Sum() = default;
    Sum(Signal _lhs, Signal _rhs) : lhs(_lhs), rhs(_rhs) { }
    float sample(float t) const {
        return lhs.sample(t) + rhs.sample(t);
    }
    Signal lhs;
    Signal rhs;
    TACT_SERIALIZE(TACT_MEMBER(lhs), TACT_MEMBER(rhs));
};

struct Product {
    Product() = default;
    Product(Signal _lhs, Signal _rhs) : lhs(_lhs), rhs(_rhs) { }
    float sample(float t) const {
        return lhs.sample(t) * rhs.sample(t);
    }
    Signal lhs;
    Signal rhs;
    TACT_SERIALIZE(TACT_MEMBER(lhs), TACT_MEMBER(rhs));
};

void myFunc(const Signal& x) {
    std::cout << x.sample(1.23f) << std::endl;
}

Product operator*(const Signal& lhs, const Signal& rhs) {
    return Product(lhs, rhs);
}

Sum operator+(const Signal& lhs, const Signal& rhs) {
    return Sum(lhs, rhs);
}

CEREAL_REGISTER_TYPE(Signal::Model<Sine>);
CEREAL_REGISTER_TYPE(Signal::Model<Sum>);
CEREAL_REGISTER_TYPE(Signal::Model<Product>);

void save(const Signal& signal, const std::string& name) {
    ofstream file(name + ".tact", std::ios::binary);
    cereal::BinaryOutputArchive archive(file);
    archive(signal); 
}

void load(Signal& signal, const std::string& name) {
    std::ifstream file(name + ".tact", std::ios::binary);
    cereal::BinaryInputArchive archive(file);
    archive(signal);
}

int main(int argc, char const *argv[])
{    

    Signal x = Sine(440);
    Signal y = Sine(200);
    Signal z = x * y;




    return 0;
}


// template <typename T>
// class Sig {
// public:

//     Sig(std::shared_ptr<T> _ptr) : ptr(std::move(_ptr)) {}

//     template <typename ...Args>
//     Sig(Args... args) :
//         ptr(std::make_shared<T>(std::forward<Args>(args)...))
//     { }

//     std::shared_ptr<T> ptr;
// };

// typedef Sig<tact::ISignal>  Signal;
// typedef Sig<tact::Envelope> Envelope;
// typedef Sig<tact::Sine>     Sine;

// template <typename L, typename R>
// Signal operator*(const Sig<L>& lhs, const Sig<R>& rhs) {
//     return Signal(lhs.ptr * rhs.ptr);
// }