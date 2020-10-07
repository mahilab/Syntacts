// MIT License
//
// Copyright (c) 2020 Mechatronics and Haptic Interfaces Lab
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Author(s): Evan Pezent (epezent@rice.edu)

#pragma once

#include <Tact/Serialization.hpp>
#include <Tact/Util.hpp>
#include <memory>
#include <string>
#include <random>
#include <map>
#include <random>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

/// A signal that simple returns the time passed to it.
struct Time {
    inline double sample(double t) const { return t; };
    constexpr double length() const { return INF; }
private:
    TACT_SERIALIZABLE
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal that emits a constant value over time.
class SYNTACTS_API Scalar
{
public:
    Scalar(double value = 1);
    double sample(double t) const;
    double length() const;
public:
    double value;
private:
    TACT_SERIALIZE(TACT_MEMBER(value));
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal that increases or decreases over time.
class SYNTACTS_API Ramp 
{
public:
    Ramp(double initial = 1, double rate = 0);
    Ramp(double initial, double final, double duration);
    double sample(double t) const;
    double length() const;
public:
    double initial;
    double rate;
    double duration;
private:
    TACT_SERIALIZE(TACT_MEMBER(initial), TACT_MEMBER(rate), TACT_MEMBER(duration));
};

///////////////////////////////////////////////////////////////////////////////

/// A signal that generates white noise.
class SYNTACTS_API Noise
{
public:
    Noise();
    double sample(double t) const;
    double length() const;
private:
    TACT_SERIALIZABLE
};

///////////////////////////////////////////////////////////////////////////////

/// A signal that returns the evaluation of an expression f(t).
class SYNTACTS_API Expression {
public:
    Expression(const std::string& expr = "sin(2*pi*100*t)");
    Expression(const Expression& other);
    ~Expression();
    double sample(double t) const;
    double length() const;
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

/// A signal that generates white noise
class SYNTACTS_API PolyBezier
{
public:
    /// Point
    struct Point {
        double t, y;
        TACT_SERIALIZE(TACT_MEMBER(t), TACT_MEMBER(y));
    };
    /// A point and left and right control points
    struct PointGroup {
        Point cpL, p, cpR;
        TACT_SERIALIZE(TACT_MEMBER(cpL), TACT_MEMBER(p), TACT_MEMBER(cpR));
    };
public:
    double sample(double t) const;
    double length() const;
    void solve();
public:
    std::vector<PointGroup> points;
    std::vector<Point> solution;
private:
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const 
    { 
        archive(TACT_MEMBER(points)); 
    }
    template<class Archive>
    void load(Archive& archive) 
    { 
        archive(TACT_MEMBER(points)); 
        solve();
    }
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal defined by an array of recorded samples (used internally for Library::importSignal).
class SYNTACTS_API Samples {
public:
    Samples();
    Samples(const std::vector<float>& samples, double sampleRate);
    double sample(double t) const;
    double length() const;
    int sampleCount() const;
    double sampleRate() const;
    double getSample(int i) const;
private:
    double m_sampleRate;
    std::shared_ptr<const std::vector<float>> m_samples;
private:
    TACT_SERIALIZE(TACT_MEMBER(m_sampleRate), TACT_MEMBER(m_samples));
};

///////////////////////////////////////////////////////////////////////////////


} // namespace tact