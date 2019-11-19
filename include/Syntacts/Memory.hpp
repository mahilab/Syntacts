#pragma once

#include <memory>

namespace tact {

///////////////////////////////////////////////////////////////////////////////

/// Smart pointer type used throughout the Syntacts library
template <typename T>
using Ptr = std::shared_ptr<T>;

/// Shorthand for std::make_shared<T>(...)
template <typename T, typename ...Args>
inline Ptr<T> create(Args... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

/// Shorthand for std::move(...)
using std::move;

///////////////////////////////////////////////////////////////////////////////

template <typename T>
class Abstract
{
};

/// general: inheritance + clone_impl implemented
template <typename Derived, typename ... Bases>
class Cloneable : public Bases...
{
public:
   virtual ~Cloneable() = default;
 
   Ptr<Derived> clone() const
   {
      return Ptr<Derived>(static_cast<Derived *>(this->clone_impl()));
   }
 
private:
   virtual Cloneable * clone_impl() const override
   {
      return new Derived(static_cast<const Derived & >(*this));
   }
};
 
///////////////////////////////////////////////////////////////////////////////
 
/// specialization: inheritance + clone_impl NOT implemented
template <typename Derived, typename ... Bases>
class Cloneable<Abstract<Derived>, Bases...> : public Bases...
{
public:
   virtual ~Cloneable() = default;
 
   Ptr<Derived> clone() const
   {
      return Ptr<Derived>(static_cast<Derived *>(this->clone_impl()));
   }
 
private:
   virtual Cloneable * clone_impl() const = 0;
};
 
///////////////////////////////////////////////////////////////////////////////
 
/// specialization: NO inheritance + clone_impl implemented
template <typename Derived>
class Cloneable<Derived>
{
public:
   virtual ~Cloneable() = default;
 
   Ptr<Derived> clone() const
   {
      return Ptr<Derived>(static_cast<Derived *>(this->clone_impl()));
   }
 
private:
   virtual Cloneable * clone_impl() const override
   {
      return new Derived(static_cast<const Derived & >(*this));
   }
};
 
///////////////////////////////////////////////////////////////////////////////
 
/// specialization: NO inheritance + clone_impl NOT implemented
template <typename Derived>
class Cloneable<Abstract<Derived>>
{
public:
   virtual ~Cloneable() = default;
 
   Ptr<Derived> clone() const
   {
      return Ptr<Derived>(static_cast<Derived *>(this->clone_impl()));
   }
 
private:
   virtual Cloneable * clone_impl() const = 0;
};



}