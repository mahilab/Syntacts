#include <Tact/Signal.hpp>

namespace tact
{

Signal::Signal() : Signal(Scalar(0)) {}

#ifndef SYNTACTS_USE_SHARED_PTR
    Signal::Signal(const Signal& other) :
        gain(other.gain),
        bias(other.bias),
        m_ptr(other.m_ptr->copy())
    {  }
    Signal& Signal::operator=(const Signal& other)
    {
        return *this = Signal(other);
    }
#endif // SYNTACTS_USE_SHARED_PTR

std::type_index Signal::typeId() const
{ 
    return m_ptr->typeId(); 
}

void* Signal::get() const
{ 
    return m_ptr->get(); 
}

int Signal::Concept::s_count = 0;

} // namespace tact