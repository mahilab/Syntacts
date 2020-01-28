namespace tact
{

inline Sequence& Sequence::operator<<(double rhs) {
    return push(rhs);
}

inline Sequence& Sequence::operator<<(Signal rhs) {
    return push(rhs);
}

inline Sequence& Sequence::operator<<(Sequence rhs) {
    return push(rhs);
}

inline Sequence operator<<(Signal lhs, Signal rhs) {
    Sequence seq;
    seq.push(lhs);
    seq.push(rhs);
    return seq;
}

inline Sequence operator<<(Signal lhs, double rhs) {
    Sequence seq;
    seq.push(lhs);
    seq.head += rhs;
    return seq;
}

inline Sequence operator<<(double lhs, Signal rhs) {
    Sequence seq;
    seq.head += lhs;
    seq.push(rhs);
    return seq;
}   

} // namespace tact
