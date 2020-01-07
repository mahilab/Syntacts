#include <Tact/General.hpp>
#include <random>
#include <ctime>
#include <misc/exprtk.hpp>
#include <iostream>
#include <Tact/Util.hpp>

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

float PolyBezier::sample(float t) const {
    if (solution.size() < 2)
        return 0; 
    else {
        auto it = std::find_if(solution.begin(), solution.end(), [&](const Point& p){return p.t > t; } );
        if (it == solution.begin() || it == solution.end())
            return 0; 
        else {
            const Point& p0 = *(std::prev(it));
            const Point& p1 = *it;   
            return remap(t, p0.t, p1.t, p0.y, p1.y);         
        }
    }
}

float PolyBezier::length() const {
    if (points.size() > 0) 
        return points.back().p.t;
    return 0;
}

void PolyBezier::solve() {
    constexpr int segments = 16;
    solution.clear();
    if (points.size() > 1) {
        std::size_t beziers = points.size() - 1;
        solution.reserve(segments * beziers);
        for (int b = 0; b < beziers; ++b) {
            for (int i = 0; i < segments; ++i) {
                auto& l = points[b];
                auto& r = points[b+1];
                float t = (float)i / (float)(segments-1);
                float u = 1.0f - t;
                float w1 = u*u*u;
                float w2 = 3*u*u*t;
                float w3 = 3*u*t*t;
                float w4 = t*t*t;
                float x = w1*l.p.t + w2*l.cpR.t + w3*r.cpL.t + w4*r.p.t;
                float y = w1*l.p.y + w2*l.cpR.y + w3*r.cpL.y + w4*r.p.y;
                solution.push_back({x,y});
            }
        }
    }
}

// https://math.stackexchange.com/questions/26846/is-there-an-explicit-form-for-cubic-b%c3%a9zier-curves/348645#348645

} // namespace tact
