

#include <Tact/Signal.hpp>
#include <random>
#include <ctime>
#include <misc/exprtk.hpp>

namespace tact
{

Scalar::Scalar(float _value) : value(_value) {}
float Scalar::sample(float t) const
{
    return value;
}
float Scalar::length() const
{
    return INF;
}

Ramp::Ramp(float _initial, float _rate) : initial(_initial), rate(_rate) {}
Ramp::Ramp(float _initial, float _final, float _span) : initial(_initial), rate((_final - _initial) / _span) {}
float Ramp::sample(float t) const { return initial + rate * t; }
float Ramp::length() const { return INF; }

Noise::Noise() {}

float Noise::sample(float t) const
{
    float R1 = (float)rand() / (float)RAND_MAX;
    float R2 = (float)rand() / (float)RAND_MAX;
    float X = (float)sqrt(-2.0f * log(R1)) * cos(2.0f * PI * R2);
    return X;
}

float Noise::length() const
{
    return INF;
}

class Expression::Impl {
public:
    Impl(const std::string& expr) :
        m_t(0)
    {
        m_table.add_variable("t",m_t);
        m_table.add_pi();
        m_expr.register_symbol_table(m_table);
        m_parser.compile(expr, m_expr);
    }
    float sample(float t) const {
        m_t = t;
        return m_expr.value();
    }
    exprtk::symbol_table<float> m_table;
    exprtk::expression<float> m_expr;
    exprtk::parser<float> m_parser;
    mutable float m_t;
};

Expression::Expression(const std::string& expr) :
    m_impl(std::move(std::make_shared<Expression::Impl>(expr)))
{

}

float Expression::sample(float t) const {
    return m_impl->sample(t);
}

float Expression::length() const {
    return INF;
}

} // namespace tact