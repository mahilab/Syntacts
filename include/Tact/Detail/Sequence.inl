namespace tact
{

inline Sequence& Sequence::operator<<(float rhs) {
    head += rhs;
    return *this;
}

inline Sequence& Sequence::operator<<(Signal rhs) {
    push(rhs);
    return *this;
}

inline Sequence& Sequence::operator<<(Sequence rhs) {
    push(rhs);
    return *this;
}

inline Sequence operator<<(Signal lhs, Signal rhs) {
    Sequence seq;
    seq.push(lhs);
    seq.push(rhs);
    return seq;
}

inline Sequence operator<<(Signal lhs, float rhs) {
    Sequence seq;
    seq.push(lhs);
    seq.head += rhs;
    return seq;
}

inline Sequence operator<<(float lhs, Signal rhs) {
    Sequence seq;
    seq.head += lhs;
    seq.push(rhs);
    return seq;
}   

} // namespace tact
