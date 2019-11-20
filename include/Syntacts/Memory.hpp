#pragma once

#include <memory>

namespace tact {

///////////////////////////////////////////////////////////////////////////////

/// Smart pointer type used throughout the Syntacts library
template <typename T>
using Ptr = std::shared_ptr<T>;

/// Shorthand for std::make_shared
template <typename T, typename ...Args>
inline Ptr<T> create(Args... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

/// Shorthand for std::dynamic_pointer_cast, expensive so avoid overuse
template <typename T1, typename T2>
inline Ptr<T1> downcast(const Ptr<T2>& t2) {
   return std::dynamic_pointer_cast<T1>(t2);
}

/// Shorthand for std::move(...)
using std::move;

///////////////////////////////////////////////////////////////////////////////

/// Generic Cloneable: inheritance + clone_impl implemented
template <typename Derived, typename Base = void>
class Cloneable : public Base
{
public:
   using Base::Base;

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

/// Used in Cloneable specializations to indicate that the type is an abstract base
template <typename T>
struct Interface { };
 
///////////////////////////////////////////////////////////////////////////////
 
/// Cloneable specialization: inheritance + clone_impl NOT implemented
template <typename Derived, typename Base>
class Cloneable<Interface<Derived>, Base> : public Base
{
public:
   using Base::Base;

   virtual ~Cloneable() = default;
 
   Ptr<Derived> clone() const
   {
      return Ptr<Derived>(static_cast<Derived *>(this->clone_impl()));
   }
 
private:
   virtual Cloneable * clone_impl() const = 0;
};
 
///////////////////////////////////////////////////////////////////////////////
 
/// Cloneable specialization: NO inheritance + clone_impl implemented
template <typename Derived>
class Cloneable<Derived, void>
{
public:
   virtual ~Cloneable() = default;
 
   Ptr<Derived> clone() const
   {
      return Ptr<Derived>(static_cast<Derived *>(this->clone_impl()));
   }
 
private:
   virtual Cloneable * clone_impl() const
   {
      return new Derived(static_cast<const Derived & >(*this));
   }
};
 
///////////////////////////////////////////////////////////////////////////////
 
/// Cloneable specialization: NO inheritance + clone_impl NOT implemented
template <typename Derived>
class Cloneable<Interface<Derived>, void>
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

} // namespace tact