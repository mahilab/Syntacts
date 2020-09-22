#include <Tact/Sequence.hpp>
#include <iostream>
#include <algorithm>

namespace tact {

Sequence::Sequence() : head(0), m_keys(0), m_length(0)
{ 
}

Sequence& Sequence::push(double t) {
    head += t;
    return *this;
}

Sequence& Sequence::push(Signal signal) {
    insert(signal, head);
    head += signal.length();
    return *this;
}

Sequence& Sequence::push(Sequence sequence) {
    insert(sequence, head);
    head += sequence.length();
    return *this;
}

Sequence& Sequence::insert(Signal signal, double t) 
{
    m_length = std::max(m_length, t + signal.length());
    m_keys.push_back({t, std::move(signal)});
    return *this;
}

Sequence& Sequence::insert(Sequence sequence, double t) {
    m_length = std::max(m_length, t + sequence.length());
    for (auto& k : sequence.m_keys)
        insert(k.signal, t + k.t);
    return *this;
}

double Sequence::sample(double t) const {
    double sample = 0;
    for (auto& k : m_keys) {
        if (t >= k.t && t <= k.t + k.signal.length())
            sample += k.signal.sample(t - k.t);
    }
    return sample;
}

double Sequence::length() const {
    return m_length;
}

void Sequence::clear() {
    m_keys.clear();
    head = 0;
    m_length = 0;
}

int Sequence::keyCount() const {
    return m_keys.size();
}

const Sequence::Key& Sequence::getKey(int idx) const {
    return m_keys[idx];
}

} // namespace tact