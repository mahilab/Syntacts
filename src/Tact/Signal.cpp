#include <Tact/Signal.hpp>

namespace tact
{

Signal::Signal() : Signal(Zero()) {}

#ifndef TACT_USE_SHARED_PTR
    Signal::Signal(const Signal& other) :
        scale(other.scale),
        offset(other.offset),
        m_ptr(other.m_ptr->copy())
    {  }
    Signal& Signal::operator=(const Signal& other)
    {
        return *this = Signal(other);
    }
#endif

std::type_index Signal::typeId() const
{ 
    return m_ptr->typeId(); 
}

void* Signal::get()  
{ 
    return m_ptr->get(); 
}

} // namespace tact