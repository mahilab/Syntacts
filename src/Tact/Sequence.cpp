#include <Tact/Sequence.hpp>
#include <iostream>

namespace tact {

Sequence::Sequence() : head(0), m_keys(0), m_length(0)
{ 
}

void Sequence::insert(Signal signal) {
    float newHead = head + signal.length();
    if (newHead > m_length)
        m_length = newHead;
    m_keys.push_back({head, std::move(signal)});
    head = newHead;
}

void Sequence::insert(float t, Signal signal) 
{
    if (t + signal.length() > m_length)
        m_length = t + signal.length();
    m_keys.push_back({t, std::move(signal)});
}

float Sequence::sample(float t) const {
    float sample = 0;
    for (auto& k : m_keys) {
        if (t >= k.t && t <= k.t + k.signal.length())
            sample += k.signal.sample(t - k.t);
    }
    return sample;
}

float Sequence::length() const {
    return m_length;
}

Sequence operator<<(Sequence lhs, Signal rhs) {
    // std::cout << "Sequence(" << lhs.length() << ") << "
    //           << "Signal(" << rhs.length() << ")" << std::endl;
    lhs.insert(rhs);
    return lhs;
}

Sequence operator<<(Sequence lhs, float rhs) {
    // std::cout << "Sequence(" << lhs.length() << ") << "
    //           << "float(" << rhs << ")" << std::endl;
    lhs.head += rhs;
    return lhs;
}

Sequence operator<<(Signal lhs, Signal rhs) {
    // std::cout << "Signal(" << lhs.length() << ") << "
    //           << "Signal(" << rhs.length() << ")" << std::endl;
    Sequence seq;
    seq.insert(lhs);
    seq.insert(rhs);
    return seq;
}

Sequence operator<<(Signal lhs, float rhs) {
    // std::cout << "Signal(" << lhs.length() << ") << "
    //           << "float(" << rhs << ")" << std::endl;
    Sequence seq;
    seq.insert(lhs);
    seq.head += rhs;
    return seq;
}

Sequence operator<<(float lhs, Signal rhs) {
    // std::cout << "float(" << lhs << ") << "
    //           << "Signal(" << rhs.length() << ")" << std::endl;
    Sequence seq;
    seq.head += lhs;
    seq.insert(rhs);
    return seq;
}



} // namespace tact