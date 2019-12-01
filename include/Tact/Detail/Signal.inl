#include <Tact/Signal.hpp>

namespace tact
{

template <typename T>
Signal::Signal(T signal) : 
    scale(1), 
    offset(0), 
#ifdef TACT_USE_MALLOC
#ifdef TACT_USE_SHARED_PTR
    m_ptr(std::make_shared<Model<T>>(std::move(signal)))
#else
    m_ptr(std::make_unique<Model<T>>(std::move(signal)))
#endif
#else
#ifdef TACT_USE_SHARED_PTR
    m_ptr(std::allocate_shared<Model<T>, Allocator<Concept>>(Allocator<Concept>(), std::move(signal)))
#else
    m_ptr(new (Signal::pool().allocate()) Model<T>(std::move(signal)))
#endif
#endif
{
#ifndef TACT_USE_MALLOC
#ifdef TACT_USE_SHARED_PTR
    static_assert((2 * sizeof(void *) + sizeof(Model<T>)) <= SIGNAL_BLOCK_SIZE, "Signal allocation would exceed SIGNAL_BLOCK SIZE");
#else
    static_assert((sizeof(Model<T>)) <= SIGNAL_BLOCK_SIZE, "Signal allocation would exceed SIGNAL_BLOCK SIZE");
#endif
#endif
}

inline float Signal::sample(float t) const
{
    return m_ptr->sample(t) * scale + offset;
}

inline void Signal::sample(const float *t, float *b, int n) const
{
    return m_ptr->sample(t, b, n, scale, offset);
}

inline float Signal::length() const
{
    return m_ptr->length();
}

template <typename T>
inline bool Signal::isType() const
{ 
    return m_ptr->typeId() == typeid(T); 
}

inline Signal::Pool& Signal::pool() {
    static Signal::Pool p;
    return p;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
Signal::Model<T>::Model() 
{} 

template <typename T>
Signal::Model<T>::Model(T model) : 
    m_model(std::move(model)) 
{ }

template <typename T>
float Signal::Model<T>::sample(float t) const 
{ 
    return m_model.sample(t); 
}

template <typename T>
void Signal::Model<T>::sample(const float* t, float* b, int n, float s, float o) const 
{ 
    for (int i = 0; i < n; ++i) 
        b[i] = m_model.sample(t[i]) * s + o;
}

template <typename T>
float Signal::Model<T>::length() const
{ 
    return m_model.length(); 
}

template <typename T>
std::type_index Signal::Model<T>::typeId() const
{ 
    static std::type_index id = typeid(T); 
    return id;
}

template <typename T>
void* Signal::Model<T>::get() 
{ 
    return &m_model; 
}

#ifndef TACT_USE_SHARED_PTR
#ifdef TACT_USE_MALLOC

template <typename T>
std::unique_ptr<Signal::Concept> Signal::Model<T>::copy() const 
{ 
    return std::make_unique<Signal::Model<T>>(*this); 
}

#else

template <typename T>
std::unique_ptr<Signal::Concept, Signal::Deleter> Signal::Model<T>::copy() const
{ 
    return std::unique_ptr<Signal::Concept, Signal::Deleter>(new (Signal::pool().allocate()) Model(*this)); 
}

#endif
#endif

///////////////////////////////////////////////////////////////////////////////

template <class Archive>
void Signal::save(Archive& archive) const {
    archive(TACT_MEMBER(scale), TACT_MEMBER(offset), TACT_MEMBER(m_ptr));
}

template <class Archive>
void Signal::load(Archive& archive) {
#if !defined TACT_USE_MALLOC && !defined TACT_USE_SHARED_PTR
    std::unique_ptr<Concept> ptr;
    archive(TACT_MEMBER(scale), TACT_MEMBER(offset), TACT_MEMBER(ptr));
    m_ptr = ptr->copy();
#else
    archive(TACT_MEMBER(scale), TACT_MEMBER(offset), TACT_MEMBER(m_ptr));
#endif
}

///////////////////////////////////////////////////////////////////////////////

} // namespace tact
