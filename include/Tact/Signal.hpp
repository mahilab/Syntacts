// MIT License
//
// Copyright (c) 2020 Mechatronics and Haptic Interfaces Lab
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Author(s): Evan Pezent (epezent@rice.edu)

#pragma once

#include <Tact/Config.hpp>
#include <Tact/General.hpp>
#include <Tact/MemoryPool.hpp>
#include <typeinfo>
#include <typeindex>

namespace tact
{

///////////////////////////////////////////////////////////////////////////////

/// An object that returns time variant samples for a length of time.
class SYNTACTS_API Signal {
public:
    /// Default Constructor.
    Signal();
    /// Type Erasing Constructor.
    template<typename T> inline Signal(T signal);   

    /// Samples the Signal at time t in seconds.
    inline double sample(double t) const;
    /// Samples the Signal at n times give by t into output buffer b.
    inline void sample(const double* t, double* b, int n) const;
    /// Returns the length of the Signal in seconds or infinity.
    inline double length() const;

    /// Returns the type_index of the underlying type-erased Signal.
    std::type_index typeId() const;
    /// Returns true if the underlying type-erased Signal is type T.
    template <typename T> inline bool isType() const;
    /// Gets a pointer to the underlying type-erased Signal type (use with caution).
    void* get() const;
    /// Gets a pointer to the underlying type-erased Signal, cast as type T (use with caution and only if you know the Signal is a T!).
    template <typename T> inline T* getAs() const;
    
    /// Returns the current count of Signals allocated in this process.
    static inline int count();

public:
    double gain;  ///< the Signal will be scaled by this amount when sampled.
    double bias;  ///< the Signal will be offset by this amount when sampled.

    /// NOT MUCH TO SEE BELOW THIS POINT EXCEPT NASTY IMPLEMENTATION DETAILS :)

#ifndef SYNTACTS_USE_SHARED_PTR
    /// Copy constructor
    Signal(const Signal& other);
    /// Move constructor
    Signal(Signal&&) noexcept = default;
    /// Assignment operator
    Signal& operator=(const Signal& other);
    /// Assignment move operator
    Signal& operator=(Signal&&) noexcept = default;
#endif

#ifdef SYNTACTS_USE_POOL
    using Pool = FriendlyStackPool<SYNTACTS_POOL_BLOCK_SIZE, SYNTACTS_POOL_MAX_BLOCKS, Signal>;
    static inline Pool& pool();
#endif

public:
    struct Concept;
    /// Unique Pointer Deleter
#ifdef SYNTACTS_USE_POOL
    struct Deleter {
        void operator()(Concept* ptr)
        { ptr->~Concept(); Signal::pool().deallocate(ptr); }
    };    
#endif
    /// Type Erasure Concept
    struct Concept {
        Concept() { s_count++; }
        virtual ~Concept() { s_count--; }
        virtual double sample(double t) const = 0;
        virtual void sample(const double* t, double* b, int n, double s, double o) const = 0;
        virtual double length() const = 0;
        virtual std::type_index typeId() const = 0;
        virtual void* get() const = 0;
#ifndef SYNTACTS_USE_SHARED_PTR
#ifdef SYNTACTS_USE_POOL
        virtual std::unique_ptr<Concept, Deleter> copy() const = 0;
#else
        virtual std::unique_ptr<Concept> copy() const = 0;
#endif
#endif
        static inline int count() {return s_count; }
        template <class Archive>
        void serialize(Archive& archive) {}
    protected:
        static int s_count;
    };
    /// Type Erasure Model
    template <typename T>
    struct Model final : Concept {
        Model();
        Model(T model);
        double sample(double t) const override;
        void sample(const double* t, double* b, int n, double s, double o) const override;
        double length() const override;
        std::type_index typeId() const override;
        void* get() const override;
#ifndef SYNTACTS_USE_SHARED_PTR
#ifdef SYNTACTS_USE_POOL
        std::unique_ptr<Concept, Deleter> copy() const override;
#else
        std::unique_ptr<Concept> copy() const override;
#endif
#endif
        T m_model;
        TACT_SERIALIZE(TACT_PARENT(Concept), TACT_MEMBER(m_model));
    };
private:
#ifdef SYNTACTS_USE_SHARED_PTR
    std::shared_ptr<const Concept> m_ptr;
#ifdef SYNTACTS_USE_POOL
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
#ifdef SYNTACTS_USE_POOL
    std::unique_ptr<Concept, Deleter> m_ptr;
#else
    std::unique_ptr<Concept> m_ptr;
#endif
#endif
private:
    friend class cereal::access;
    template <class Archive> void save(Archive& archive) const;
    template <class Archive> void load(Archive& archive);
};

///////////////////////////////////////////////////////////////////////////////

} // namespace tact

#include <Tact/Detail/Signal.inl> // inline and template implementations