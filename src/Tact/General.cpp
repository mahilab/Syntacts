#include <Tact/General.hpp>
#include <ctime>
#include <misc/exprtk.hpp>
#include <iostream>
#include <Tact/Util.hpp>

namespace tact
{

Scalar::Scalar(double _value) : value(_value) {}
double Scalar::sample(double t) const
{
    return value;
}

double Scalar::length() const
{
    return INF;
}

Ramp::Ramp(double _initial, double _rate) : initial(_initial), rate(_rate), duration(INF) {}
Ramp::Ramp(double _initial, double _final, double _duration) : initial(_initial), rate((_final - _initial) / _duration), duration(_duration) {}
double Ramp::sample(double t) const { return initial + rate * t; }
double Ramp::length() const { return duration; }

Noise::Noise()
{ }

double Noise::sample(double t) const
{
    static std::uniform_real_distribution<double> dist(-1,1);
    static std::mt19937 rgen;
    return dist(rgen);
}

double Noise::length() const
{
    return INF;
}

class Expression::Impl
{
public:
    Impl() : m_t(0) {}

    double sample(double t) const
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
    exprtk::symbol_table<double> m_table;
    exprtk::expression<double> m_expr;
    exprtk::parser<double> m_parser;
    std::string m_str;
    mutable double m_t;
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

double Expression::sample(double t) const
{
    return m_impl->sample(t);
}

double Expression::length() const
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

double PolyBezier::sample(double t) const {
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

double PolyBezier::length() const {
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
                double t = (double)i / (double)(segments-1);
                double u = 1.0f - t;
                double w1 = u*u*u;
                double w2 = 3*u*u*t;
                double w3 = 3*u*t*t;
                double w4 = t*t*t;
                double x = w1*l.p.t + w2*l.cpR.t + w3*r.cpL.t + w4*r.p.t;
                double y = w1*l.p.y + w2*l.cpR.y + w3*r.cpL.y + w4*r.p.y;
                solution.push_back({x,y});
            }
        }
    }
}

// https://math.stackexchange.com/questions/26846/is-there-an-explicit-form-for-cubic-b%c3%a9zier-curves/348645#348645


Samples::Samples() : m_sampleRate(44100), m_samples() { }

Samples::Samples(const std::vector<float>& samples, double sampleRate) :
    m_samples(std::make_shared<std::vector<float>>(std::move(samples))),
    m_sampleRate(sampleRate)
{ }

double Samples::sample(double t) const {
    std::size_t i = static_cast<std::size_t>(t * m_sampleRate);
    if (i < m_samples->size() - 1) 
        return m_samples->operator[](i);
    return 0;
}

double Samples::length() const {
    return static_cast<double>(m_samples->size()) / m_sampleRate;
}

int Samples::sampleCount() const {
    return static_cast<int>(m_samples->size());
}

double Samples::sampleRate() const {
    return m_sampleRate;
}

double Samples::getSample(int i) const {
    return m_samples->operator[](i);
}

} // namespace tact
