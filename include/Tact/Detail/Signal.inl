#include <Tact/Signal.hpp>

namespace tact
{

template <typename T>
Signal::Signal(T signal) : 
    gain(1), 
    bias(0), 
#ifndef SYNTACTS_USE_POOL
#ifdef SYNTACTS_USE_SHARED_PTR
    m_ptr(std::make_shared<Model<T>>(std::move(signal)))
#else
    m_ptr(std::make_unique<Model<T>>(std::move(signal)))
#endif
#else
#ifdef SYNTACTS_USE_SHARED_PTR
    m_ptr(std::allocate_shared<Model<T>, Allocator<Concept>>(Allocator<Concept>(), std::move(signal)))
#else
    m_ptr(new (Signal::pool().allocate()) Model<T>(std::move(signal)))
#endif
#endif
{
#ifdef SYNTACTS_USE_POOL
#ifdef SYNTACTS_USE_SHARED_PTR
    static_assert((2 * sizeof(void *) + sizeof(Model<T>)) <= SYNTACTS_POOL_BLOCK_SIZE, "Signal allocation would exceed SIGNAL_BLOCK SIZE");
#else
    static_assert((sizeof(Model<T>)) <= SYNTACTS_POOL_BLOCK_SIZE, "Signal allocation would exceed SIGNAL_BLOCK SIZE");
#endif
#endif
}

inline double Signal::sample(double t) const
{
    return m_ptr->sample(t) * gain + bias;
}

inline void Signal::sample(const double *t, double *b, int n) const
{
    return m_ptr->sample(t, b, n, gain, bias);
}

inline double Signal::length() const
{
    return m_ptr->length();
}

template <typename T>
inline bool Signal::isType() const
{ 
    return m_ptr->typeId() == typeid(T); 
}

template <typename T> inline T* Signal::getAs() const {
    return static_cast<T*>(m_ptr->get());
}

#ifdef SYNTACTS_USE_POOL
inline Signal::Pool& Signal::pool() {
    static Signal::Pool p;
    return p;
}
#endif

inline int Signal::count() {
    return Concept::count();
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
double Signal::Model<T>::sample(double t) const 
{ 
    return m_model.sample(t); 
}

template <typename T>
void Signal::Model<T>::sample(const double* t, double* b, int n, double s, double o) const 
{ 
    for (int i = 0; i < n; ++i) 
        b[i] = m_model.sample(t[i]) * s + o;
}

template <typename T>
double Signal::Model<T>::length() const
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
void* Signal::Model<T>::get() const
{ 
    return (void*)&m_model; 
}

#ifndef SYNTACTS_USE_SHARED_PTR
#ifndef SYNTACTS_USE_POOL

template <typename T>
std::unique_ptr<Signal::Concept> Signal::Model<T>::copy() const 
{ 
    s_count++;
    return std::make_unique<Signal::Model<T>>(*this); 
}

#else

template <typename T>
std::unique_ptr<Signal::Concept, Signal::Deleter> Signal::Model<T>::copy() const
{ 
    s_count++;
    return std::unique_ptr<Signal::Concept, Signal::Deleter>(new (Signal::pool().allocate()) Model(*this)); 
}

#endif
#endif

///////////////////////////////////////////////////////////////////////////////

template <class Archive>
void Signal::save(Archive& archive) const {
    archive(TACT_MEMBER(gain), TACT_MEMBER(bias), TACT_MEMBER(m_ptr));
}

template <class Archive>
void Signal::load(Archive& archive) {
#if defined SYNTACTS_USE_POOL && !defined SYNTACTS_USE_SHARED_PTR
    std::unique_ptr<Concept> ptr;
    archive(TACT_MEMBER(gain), TACT_MEMBER(bias), TACT_MEMBER(ptr));
    m_ptr = ptr->copy();
#else
    archive(TACT_MEMBER(gain), TACT_MEMBER(bias), TACT_MEMBER(m_ptr));
#endif
}

///////////////////////////////////////////////////////////////////////////////

} // namespace tact
