#pragma once

#include <Tact/Macros.hpp>
#include <Tact/Util.hpp>
#include <memory>
#include <string>
#include <random>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

/// A signal that simple returns the time passed to it
struct Time {
    inline float sample(float t) const { return t; };
    constexpr float length() const { return INF; }
private:
    TACT_SERIALIZABLE
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal that emits a constant value over time
class SYNTACTS_API Scalar
{
public:
    Scalar(float value = 1);
    float sample(float t) const;
    float length() const;
public:
    float value;
private:
    TACT_SERIALIZE(TACT_MEMBER(value));
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal that increases or decreases over time
class SYNTACTS_API Ramp 
{
public:
    Ramp(float initial = 1, float rate = 0);
    Ramp(float initial, float final, float span);
    float sample(float t) const;
    float length() const;
public:
    float initial;
    float rate;
private:
    TACT_SERIALIZE(TACT_MEMBER(initial), TACT_MEMBER(rate));
};

///////////////////////////////////////////////////////////////////////////////

/// A signal that generates white noise
class SYNTACTS_API Noise
{
public:
    Noise();
    float sample(float t) const;
    float length() const;
private:
    TACT_SERIALIZABLE
};

///////////////////////////////////////////////////////////////////////////////

class SYNTACTS_API Expression {
public:
    Expression(const std::string& expr = "sin(2*pi*100*t)");
    Expression(const Expression& other);
    ~Expression();
    float sample(float t) const;
    float length() const;
    bool setExpression(const std::string& expr);
    const std::string& getExpression() const;
    bool operator=(const std::string& expr);
private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
private:
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const 
    { 
        std::string expr = getExpression();
        archive(TACT_MEMBER(expr)); 
    }
    template<class Archive>
    void load(Archive& archive) 
    { 
        std::string expr;
        archive(TACT_MEMBER(expr)); 
        setExpression(expr); 
    }
};

///////////////////////////////////////////////////////////////////////////////

} // namespace tact