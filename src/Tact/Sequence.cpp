#include <Tact/Sequence.hpp>
#include <iostream>
#include <algorithm>

namespace tact {

Sequence::Sequence() : head(0), m_keys(0), m_length(0)
{ 
}

void Sequence::push(Signal signal) {
    insert(signal, head);
    head += signal.length();
}

void Sequence::push(Sequence sequence) {
    insert(sequence, head);
    head += sequence.length();
}

void Sequence::insert(Signal signal, float t) 
{
    m_length = std::max(m_length, t + signal.length());
    m_keys.push_back({t, std::move(signal)});
}

void Sequence::insert(Sequence sequence, float t) {
    m_length = std::max(m_length, t + sequence.length());
    for (auto& k : sequence.m_keys)
        insert(k.signal, t + k.t);
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

} // namespace tact