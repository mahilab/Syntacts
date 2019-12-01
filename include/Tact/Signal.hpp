#pragma once

#include <Tact/General.hpp>
#include <Tact/MemoryPool.hpp>
#include <typeinfo>
#include <typeindex>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

// If uncommented, Signal will use default malloc instead of memory pool
// #define TACT_USE_MALLOC     

// If uncommented, Signal will use shared pointers instead of unique pointers
// #define TACT_USE_SHARED_PTR 

///////////////////////////////////////////////////////////////////////////////

/// The size of memory blocks available to store Signals
constexpr std::size_t SIGNAL_BLOCK_SIZE  = 64;
/// The maximum number of signals that can simultaneously exist
constexpr std::size_t MAX_SIGNALS = 1024;

///////////////////////////////////////////////////////////////////////////////

/// An object that returns time variant samples for a length of time
class SYNTACTS_API Signal {
public:
    /// Default constructor
    Signal();
    /// Constructor
    template<typename T> inline Signal(T signal);
     
#ifndef TACT_USE_SHARED_PTR
    /// Copy constructor
    Signal(const Signal& other);
    /// Move constructor
    Signal(Signal&&) noexcept = default;
    /// Assignment operator
    Signal& operator=(const Signal& other);
    /// Assignment move operator
    Signal& operator=(Signal&&) noexcept = default;
#endif

    /// Samples the Signal at time t in seconds
    inline float sample(float t) const;
    /// Samples the Signal at n times give by t into output buffer b
    inline void sample(const float* t, float* b, int n) const;
    /// Returns the length of the Signal in seconds or infinity
    inline float length() const;
    /// Returns the type_index of the underlying Signal
    std::type_index typeId() const;
    /// Returns true if the underlying Signal is type T
    template <typename T> inline bool isType() const;
    /// Gets a pointer to the underlying Signal type (use with caution)
    void* get();

public:
    using Pool = FriendlyStackPool<SIGNAL_BLOCK_SIZE, MAX_SIGNALS, Signal>;
    /// Gets a reference to the Signal memory pool
    static inline Pool& pool();
public:
    float scale;  // the signal will be scaled by this amount when sampled
    float offset; // the signal will be offset by this amount when sampled
public:
    struct Concept;
    /// Unique Pointer Deleter
    struct Deleter {
        void operator()(Concept* ptr)
        { ptr->~Concept(); Signal::pool().deallocate(ptr); }
    };    
    /// Type Erasure Concept
    struct Concept {
        Concept() = default;
        virtual ~Concept() = default;
        virtual float sample(float t) const = 0;
        virtual void sample(const float* t, float* b, int n, float s, float o) const = 0;
        virtual float length() const = 0;
        virtual std::type_index typeId() const = 0;
        virtual void* get() = 0;
#ifndef TACT_USE_SHARED_PTR
#ifdef TACT_USE_MALLOC
        virtual std::unique_ptr<Concept> copy() const = 0;
#else
        virtual std::unique_ptr<Concept, Deleter> copy() const = 0;
#endif
#endif
        template <class Archive>
        void serialize(Archive& archive) {}
    };
    /// Type Erasure Model
    template <typename T>
    struct Model final : Concept {
        Model();
        Model(T model);
        float sample(float t) const override;
        void sample(const float* t, float* b, int n, float s, float o) const override;
        float length() const override;
        std::type_index typeId() const override;
        void* get() override;
#ifndef TACT_USE_SHARED_PTR
#ifdef TACT_USE_MALLOC
        std::unique_ptr<Concept> copy() const override;
#else
        std::unique_ptr<Concept, Deleter> copy() const override;
#endif
#endif
        T m_model;
        TACT_SERIALIZE(TACT_PARENT(Concept), TACT_MEMBER(m_model));
    };
private:
#ifdef TACT_USE_SHARED_PTR
    std::shared_ptr<const Concept> m_ptr;
#ifndef TACT_USE_MALLOC
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
#endif
#else
#ifdef TACT_USE_MALLOC
    std::unique_ptr<Concept> m_ptr;
#else
    std::unique_ptr<Concept, Deleter> m_ptr;
#endif
#endif
    friend class cereal::access;
    template <class Archive> void save(Archive& archive) const;
    template <class Archive> void load(Archive& archive);
};

///////////////////////////////////////////////////////////////////////////////

} // namespace tact

#include <Tact/Detail/Signal.inl> // inline and template implementations