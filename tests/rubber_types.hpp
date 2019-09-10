#include <memory>
// #include <any>
namespace rubber_types
{

namespace detail
{

template <class T>
class Holder
{
public:
    using Data = T;
    Holder(T obj) : data_(std::move(obj)) {}
    virtual ~Holder() = default;
    T &get_from_rubber_types_holder_() { return data_; }
    const T &get_from_rubber_types_holder_() const { return data_; }

private:
    T data_;
};

// Merged concept models chaining inheritance of the above holder and models.
// This template meta-function peels off inheritance layers until it reaches
// the above defined holder.
template <class Holder>
struct unwrap_holder;
template <class T>
struct unwrap_holder<Holder<T>>
{
    using type = Holder<T>;
};
template <template <class> class Model, class Holder>
struct unwrap_holder<Model<Holder>>
{
    using type = Holder;
};
template <class T>
using UnwrapHolder = typename unwrap_holder<T>::type;

template <class Concept_, template <class> class Model>
class Container
{
public:
    using Concept = Concept_;

    Container(){};

    template <class T>
    Container(T obj) : self_(std::make_shared<Model<Holder<T>>>(std::move(obj))) {}

    const Concept &get_from_rubber_types_container_() const
    {
        return *self_.get();
    }
    Concept &get_from_rubber_types_container_()
    {
        return const_cast<Concept &>(*self_.get());
    }

private:
    std::shared_ptr<const Concept> self_;
};

// The same peeling for the concept's external interface to reach the
// container.
template <class Container>
struct unwrap_container;
template <class Concept, template <class> class Model>
struct unwrap_container<Container<Concept, Model>>
{
    using type = Container<Concept, Model>;
};
template <template <class> class ExternalInterface, class Container>
struct unwrap_container<ExternalInterface<Container>>
{
    using type = Container;
};
template <class T>
using UnwrapContainer = typename unwrap_container<T>::type;

// Helpers for spec merging.
template <class Spec>
using ConceptOf = typename Spec::Concept;

template <class Spec, class Holder>
using ModelOf = typename Spec::template Model<Holder>;

template <class Spec, class Container>
using ExternalInterfaceOf =
    typename Spec::template ExternalInterface<Container>;

// Right-fold. A higher order functino, that maps a two argument template-meta
// function and a list of arguments to the following expression:
//     Rfold<F, a1, a2, ...> = F<a1, F<a2, ...> >
template <template <class, class> class Func, class Arg, class... Args>
struct rfold
{
    using type = Func<Arg, typename rfold<Func, Args...>::type>;
};
template <template <class, class> class Func, class L, class R>
struct rfold<Func, L, R>
{
    using type = Func<L, R>;
};
template <template <class, class> class Func, class... Args>
using Rfold = typename rfold<Func, Args...>::type;

} // namespace detail

// Get the held value out of a model.
// Use this function inside your model-definitions to access the wrapped
// value.
template <class Model>
const typename detail::UnwrapHolder<Model>::Data &
model_get(const Model *this_)
{
    using Holder = detail::UnwrapHolder<Model>;
    return this_->Holder::get_from_rubber_types_holder_();
}
template <class Model>
const typename detail::UnwrapHolder<Model>::Data &
model_get(const Model &this_)
{
    using Holder = detail::UnwrapHolder<Model>;
    return this_.Holder::get_from_rubber_types_holder_();
}
//////////////////////////////////////////////////////
template <class Model>
typename detail::UnwrapHolder<Model>::Data &
model_get(Model &this_)
{
    using Holder = detail::UnwrapHolder<Model>;
    return this_.Holder::get_from_rubber_types_holder_();
}
//////////////////////////////////////////////////////
// Get the held value out of an external interface.
// Use this function inside your external interface-definitions to access the
// model.
template <class ExternalInterface>
const typename detail::UnwrapContainer<ExternalInterface>::Concept &
interface_get(const ExternalInterface *this_)
{
    using Container = detail::UnwrapContainer<ExternalInterface>;
    return this_->Container::get_from_rubber_types_container_();
}
template <class ExternalInterface>
const typename detail::UnwrapContainer<ExternalInterface>::Concept &
interface_get(const ExternalInterface &this_)
{
    using Container = detail::UnwrapContainer<ExternalInterface>;
    return this_.Container::get_from_rubber_types_container_();
}

template <class ExternalInterface>
typename detail::UnwrapContainer<ExternalInterface>::Concept &
interface_get(ExternalInterface &this_)
{
    using Container = detail::UnwrapContainer<ExternalInterface>;
    return this_.Container::get_from_rubber_types_container_();
}
// Merge a list of specs into one spec.
template <class... Specs>
struct MergeSpecs
{
    struct Concept : public virtual detail::ConceptOf<Specs>...
    {
    };

    template <class Holder>
    struct Model : public detail::Rfold<detail::ModelOf, Specs..., Holder>,
                   public virtual Concept
    {
        using Base = detail::Rfold<detail::ModelOf, Specs..., Holder>;
        using Base::Base;
    };

    template <class Container>
    struct ExternalInterface
        : public detail::Rfold<detail::ExternalInterfaceOf, Specs...,
                               Container>
    {
        using Base =
            detail::Rfold<detail::ExternalInterfaceOf, Specs..., Container>;
        using Base::Base;
    };
};

template <class Spec>
struct MergeSpecs<Spec> : public Spec
{
};

namespace detail
{

// Construct a type-erasure out of a given spec.
template <class Spec_>
class TypeErasureSingleSpec : public Spec_::template ExternalInterface<detail::Container<typename Spec_::Concept, Spec_::template Model>>
{
    using Base = typename Spec_::template ExternalInterface<detail::Container<typename Spec_::Concept, Spec_::template Model>>;

public:
    using Base::Base;
    using Spec = Spec_;
};

} // namespace detail

// Generate a type-erasure from the given list of specs.
template <class... Specs>
using TypeErasure = detail::TypeErasureSingleSpec<MergeSpecs<Specs...>>;

// Merge existing concepts (type-erasure classes) into a single one.
template <class... Concepts>
using MergeConcepts = TypeErasure<typename Concepts::Spec...>;

} // namespace rubber_types