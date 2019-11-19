#include <memory>

template <typename T>
class abstract_method
{
};

// general: inheritance + clone_impl implemented
template <typename Derived, typename ... Bases>
class clone_inherit : public Bases...
{
public:
   virtual ~clone_inherit() = default;
 
   std::shared_ptr<Derived> clone() const
   {
      return std::shared_ptr<Derived>(static_cast<Derived *>(this->clone_impl()));
   }
 
private:
   virtual clone_inherit * clone_impl() const override
   {
      return new Derived(static_cast<const Derived & >(*this));
   }
};
 
///////////////////////////////////////////////////////////////////////////////
 
// specialization: inheritance + clone_impl NOT implemented
template <typename Derived, typename ... Bases>
class clone_inherit<abstract_method<Derived>, Bases...> : public Bases...
{
public:
   virtual ~clone_inherit() = default;
 
   std::shared_ptr<Derived> clone() const
   {
      return std::shared_ptr<Derived>(static_cast<Derived *>(this->clone_impl()));
   }
 
private:
   virtual clone_inherit * clone_impl() const = 0;
};
 
///////////////////////////////////////////////////////////////////////////////
 
// specialization: NO inheritance + clone_impl implemented
template <typename Derived>
class clone_inherit<Derived>
{
public:
   virtual ~clone_inherit() = default;
 
   std::shared_ptr<Derived> clone() const
   {
      return std::shared_ptr<Derived>(static_cast<Derived *>(this->clone_impl()));
   }
 
private:
   virtual clone_inherit * clone_impl() const override
   {
      return new Derived(static_cast<const Derived & >(*this));
   }
};
 
///////////////////////////////////////////////////////////////////////////////
 
// specialization: NO inheritance + clone_impl NOT implemented
template <typename Derived>
class clone_inherit<abstract_method<Derived>>
{
public:
   virtual ~clone_inherit() = default;
 
   std::shared_ptr<Derived> clone() const
   {
      return std::shared_ptr<Derived>(static_cast<Derived *>(this->clone_impl()));
   }
 
private:
   virtual clone_inherit * clone_impl() const = 0;
};

class cloneable
   : public clone_inherit<abstract_method<cloneable>>
{
public:
   virtual int getA() = 0;
};
 
///////////////////////////////////////////////////////////////////////////////
 
class abstracted
   : public clone_inherit<abstract_method<abstracted>, cloneable>
{
public:
   virtual int getB() = 0;
};
 
///////////////////////////////////////////////////////////////////////////////
 
class concrete
   : public clone_inherit<concrete, abstracted>
{
public:
   int getA() override {
      return 1;
   }
   int getB() override  {
      return 2;
   }
};

int main()
{
   std::shared_ptr<concrete> c = std::make_shared<concrete>();
   std::shared_ptr<concrete> cc = c->clone();
 
   std::shared_ptr<abstracted> aa = c->clone();
 
   std::shared_ptr<cloneable> pp = c->clone();
}
