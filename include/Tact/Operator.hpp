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

#include <Tact/Signal.hpp>

namespace tact {

///////////////////////////////////////////////////////////////////////////////

/// A signal which is the result of operating on two other Signals
struct IOperator {
    IOperator() = default;
    IOperator(Signal lhs, Signal rhs);
public:
    Signal lhs, rhs;
private:
    TACT_SERIALIZE(TACT_MEMBER(lhs), TACT_MEMBER(rhs));
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal which is the sum or difference of two other signals
struct Sum : public IOperator {
    using IOperator::IOperator;
    double sample(double t) const;
    double length() const;
private:
    TACT_SERIALIZE(TACT_PARENT(IOperator));
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal which is the product of two other signals
struct Product : public IOperator {
    using IOperator::IOperator;
    double sample(double t) const;
    double length() const;
private:
    TACT_SERIALIZE(TACT_PARENT(IOperator));
};

///////////////////////////////////////////////////////////////////////////////

inline Signal operator*(Signal lhs, Signal rhs);
inline Signal operator*(double lhs, Signal rhs);
inline Signal operator*(Signal lhs, double rhs);
inline Signal& operator*=(Signal& lhs, double rhs);

inline Signal operator+(Signal lhs, Signal rhs);
inline Signal operator+(double lhs, Signal rhs);
inline Signal operator+(Signal lhs, double rhs);
inline Signal& operator+=(Signal& lhs, double rhs);

inline Signal operator-(Signal lhs, Signal rhs);
inline Signal operator-(double lhs, Signal rhs);
inline Signal operator-(Signal lhs, double rhs);
inline Signal& operator-=(Signal& lhs, double rhs);

inline Signal operator-(Signal lhs);

///////////////////////////////////////////////////////////////////////////////

} // namespace tact

#include <Tact/Detail/Operator.inl>
