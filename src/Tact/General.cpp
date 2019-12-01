#include <Tact/General.hpp>
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

Noise::Noise()
{ }

float Noise::sample(float t) const
{
    static std::uniform_real_distribution<float> dist(-1,1);
    static std::mt19937 rgen;
    return dist(rgen);
}

float Noise::length() const
{
    return INF;
}

class Expression::Impl
{
public:
    Impl() : m_t(0) {}

    float sample(float t) const
    {
        m_t = t;
        return m_expr.value();
    }
    bool setExpression(const std::string &expr)
    {
        m_str = expr;
        m_table.add_variable("t", m_t);
        m_table.add_pi();
        m_expr.register_symbol_table(m_table);
        return m_parser.compile(m_str, m_expr);
    }
    exprtk::symbol_table<float> m_table;
    exprtk::expression<float> m_expr;
    exprtk::parser<float> m_parser;
    std::string m_str;
    mutable float m_t;
};

Expression::Expression(const std::string &expr) : m_impl(std::move(std::make_unique<Expression::Impl>()))
{
    setExpression(expr);
}

Expression::~Expression() {
    
}

Expression::Expression(const Expression& other) :
    Expression(other.getExpression())
{ }

float Expression::sample(float t) const
{
    return m_impl->sample(t);
}

float Expression::length() const
{
    return INF;
}

bool Expression::setExpression(const std::string &expr)
{
    return m_impl->setExpression(expr);
}

const std::string& Expression::getExpression() const {
    return m_impl->m_str;
}

bool Expression::operator=(const std::string &expr)
{
    return setExpression(expr);
}

} // namespace tact
