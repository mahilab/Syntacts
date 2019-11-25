#pragma once

#include <Tact/Macros.hpp>
#include <Tact/MemoryPool.hpp>
#include <Tact/Util.hpp>
#include <memory>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

/// All Signals must occupy less than or equal this - 16 bytes (two void*)
constexpr std::size_t SIGNAL_BLOCK_SIZE  = 128;
/// The maximum number of signals that can simultaneously exist.
constexpr std::size_t MAX_SIGNALS = 512;

///////////////////////////////////////////////////////////////////////////////

/// An signal that returns zero and has zero length
struct ZeroSignal {
    constexpr float sample(float t) const { return 0; }
    constexpr float length() const { return 0; }
};

///////////////////////////////////////////////////////////////////////////////

/// An object that returns time variant samples for a length of time
class SYNTACTS_API Signal {
public:
    /// Constructor
    template<typename T>
    Signal(T signal) : scale(1), offset(0),
#ifdef TACT_USE_MALLOC
    m_ptr(std::make_shared<Model<T>>(std::move(signal))) { }
#else
    m_ptr(std::allocate_shared<Model<T>, Allocator<Concept>>(Allocator<Concept>(), std::move(signal)))  
    { static_assert((2*sizeof(void*)+sizeof(Model<T>))<=SIGNAL_BLOCK_SIZE,"Signal allocation would exceed SIGNAL_BLOCK SIZE"); }    
#endif 
    /// Default constructor
    Signal() : Signal(ZeroSignal()) {}
    /// Samples the Signal at time t in seconds
    inline float sample(float t) const 
    { return m_ptr->sample(t) * scale + offset; }
    inline void sample(const float* t, float* b, int n) const
    { return m_ptr->sample(t,b,n,scale,offset); }
    /// Returns the length of the Signal in seconds
    inline float length() const
    { return m_ptr->length(); }
public:
    float scale;
    float offset;
public:
    struct Concept {
        Concept() = default;
        virtual ~Concept() = default;
        virtual float sample(float t) const = 0;
        virtual void sample(const float* t, float* b, int n, float s, float o) const = 0;
        virtual float length() const = 0;
        template <class Archive>
        void serialize(Archive& archive) {}
    };
    /// Model
    template <typename T>
    struct Model final : Concept {
        Model() = default;
        Model(T model) : m_model(std::move(model)) { }
        float sample(float t) const override 
        { return m_model.sample(t); }
        virtual void sample(const float* t, float* b, int n, float s, float o) const override
        { for (int i = 0; i < n; ++i) { b[i] = m_model.sample(t[i]) * s + o; } }
        float length() const override
        { return m_model.length(); }
        T m_model;
        TACT_SERIALIZE(TACT_PARENT(Concept), TACT_MEMBER(m_model));
    };
private:
    std::shared_ptr<const Concept> m_ptr;
private:
    TACT_SERIALIZE(TACT_MEMBER(scale), TACT_MEMBER(offset), TACT_MEMBER(m_ptr));
public:
#ifndef TACT_USE_MALLOC
    /// Allocator
    template<typename T>
    struct Allocator
    {
        typedef T value_type;
        Allocator() noexcept {};
        template<typename U>
        Allocator(const Allocator<U>& other) throw() {};
        T* allocate(std::size_t n, const void* hint = 0)
        { return static_cast<T*>(Signal::pool().allocate()); }
        void deallocate(T* ptr, std::size_t n)
        { Signal::pool().deallocate((void*)ptr); }
    };
    static inline auto& pool() {
        static StackPool<SIGNAL_BLOCK_SIZE, MAX_SIGNALS> p;
        // static HeapPool p(SIGNAL_BLOCK_SIZE, MAX_SIGNALS);
        return p;
    }
#endif
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal that emits a constant value over time
class SYNTACTS_API Scalar
{
public:
    Scalar(float value = 1);
    float sample(float t) const;
    float length() const;
public:
    float value;
private:
    TACT_SERIALIZE(TACT_PARENT(ISignal), TACT_MEMBER(value));
};

///////////////////////////////////////////////////////////////////////////////

/// A Signal that increases or decreases over time
class SYNTACTS_API Ramp 
{
public:
    Ramp(float initial = 1, float rate = 0);
    Ramp(float initial, float final, float span);
    float sample(float t) const;
    float length() const;
public:
    float initial;
    float rate;
private:
    TACT_SERIALIZE(TACT_PARENT(ISignal), TACT_MEMBER(initial), TACT_MEMBER(rate));
};

///////////////////////////////////////////////////////////////////////////////

/// A signal that generates white noise
class SYNTACTS_API Noise
{
public:
    Noise();
    float sample(float t) const;
    float length() const;
private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& archive) {}
};

///////////////////////////////////////////////////////////////////////////////


} // namespace tact

