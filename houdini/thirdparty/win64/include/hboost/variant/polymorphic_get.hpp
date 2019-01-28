//-----------------------------------------------------------------------------
// boost variant/polymorphic_get.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2013-2015 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_VARIANT_POLYMORPHIC_GET_HPP
#define HBOOST_VARIANT_POLYMORPHIC_GET_HPP

#include <exception>

#include "hboost/config.hpp"
#include "hboost/detail/workaround.hpp"
#include "hboost/static_assert.hpp"
#include "hboost/throw_exception.hpp"
#include "hboost/utility/addressof.hpp"
#include "hboost/variant/variant_fwd.hpp"
#include "hboost/variant/get.hpp"

#include "hboost/type_traits/add_reference.hpp"
#include "hboost/type_traits/add_pointer.hpp"
#include "hboost/type_traits/is_base_of.hpp"

namespace hboost {

//////////////////////////////////////////////////////////////////////////
// class bad_polymorphic_get
//
// The exception thrown in the event of a failed get of a value.
//
class HBOOST_SYMBOL_VISIBLE bad_polymorphic_get
    : public bad_get
{
public: // std::exception implementation

    virtual const char * what() const HBOOST_NOEXCEPT_OR_NOTHROW
    {
        return "hboost::bad_polymorphic_get: "
               "failed value get using hboost::polymorphic_get";
    }

};

//////////////////////////////////////////////////////////////////////////
// function template get<T>
//
// Retrieves content of given variant object if content is of type T.
// Otherwise: pointer ver. returns 0; reference ver. throws bad_get.
//

namespace detail { namespace variant {


///////////////////////////////////////////////////////////////////////////////////////////////////
// polymorphic metafunctions to detect index of a value
//

template <class Types, class T>
struct element_polymorphic_iterator_impl :
    hboost::mpl::find_if<
        Types,
        hboost::mpl::or_<
            variant_element_functor<hboost::mpl::_1, T>,
            variant_element_functor<hboost::mpl::_1, typename hboost::remove_cv<T>::type >,
            hboost::is_base_of<T, hboost::mpl::_1>
        >
    >
{};

template <class Variant, class T>
struct holds_element_polymorphic :
    hboost::mpl::not_<
        hboost::is_same<
            typename hboost::mpl::end<typename Variant::types>::type,
            typename element_polymorphic_iterator_impl<typename Variant::types, typename hboost::remove_reference<T>::type >::type
        >
    >
{};

// (detail) class template get_polymorphic_visitor
//
// Generic static visitor that: if the value is of the specified
// type or of a type derived from specified, returns a pointer
// to the value it visits; else a null pointer.
//
template <typename Base>
struct get_polymorphic_visitor
{
private: // private typedefs
    typedef get_polymorphic_visitor<Base>       this_type;
    typedef typename add_pointer<Base>::type    pointer;
    typedef typename add_reference<Base>::type  reference;

    pointer get(reference operand, hboost::true_type) const HBOOST_NOEXCEPT
    {
        return hboost::addressof(operand);
    }

    template <class T>
    pointer get(T&, hboost::false_type) const HBOOST_NOEXCEPT
    {
        return static_cast<pointer>(0);
    }

public: // visitor interfaces
    typedef pointer result_type;

    template <typename U>
    pointer operator()(U& operand) const HBOOST_NOEXCEPT
    {
        typedef hboost::integral_constant<
            bool,
            hboost::mpl::or_<
                hboost::is_base_of<Base, U>,
                hboost::is_same<Base, U>,
                hboost::is_same<typename hboost::remove_cv<Base>::type, U >
            >::value
        > tag_t;

        return this_type::get(operand, tag_t());
    }
};

}} // namespace detail::variant

#ifndef HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE
#   if !HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x0551))
#       define HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(t)
#   else
#       define HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(t)  \
        , t* = 0
#   endif
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// polymorphic_relaxed_get
//

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_pointer<U>::type
polymorphic_relaxed_get(
      hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >* operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    ) HBOOST_NOEXCEPT
{
    typedef typename add_pointer<U>::type U_ptr;
    if (!operand) return static_cast<U_ptr>(0);

    detail::variant::get_polymorphic_visitor<U> v;
    return operand->apply_visitor(v);
}

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_pointer<const U>::type
polymorphic_relaxed_get(
      const hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >* operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    ) HBOOST_NOEXCEPT
{
    typedef typename add_pointer<const U>::type U_ptr;
    if (!operand) return static_cast<U_ptr>(0);

    detail::variant::get_polymorphic_visitor<const U> v;
    return operand->apply_visitor(v);
}

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_reference<U>::type
polymorphic_relaxed_get(
      hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >& operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    )
{
    typedef typename add_pointer<U>::type U_ptr;
    U_ptr result = polymorphic_relaxed_get<U>(&operand);

    if (!result)
        hboost::throw_exception(bad_polymorphic_get());
    return *result;
}

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_reference<const U>::type
polymorphic_relaxed_get(
      const hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >& operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    )
{
    typedef typename add_pointer<const U>::type U_ptr;
    U_ptr result = polymorphic_relaxed_get<const U>(&operand);

    if (!result)
        hboost::throw_exception(bad_polymorphic_get());
    return *result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// polymorphic_strict_get
//

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_pointer<U>::type
polymorphic_strict_get(
      hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >* operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    ) HBOOST_NOEXCEPT
{
    HBOOST_STATIC_ASSERT_MSG(
        (hboost::detail::variant::holds_element_polymorphic<hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >, U >::value),
        "hboost::variant does not contain specified type U, "
        "call to hboost::polymorphic_get<U>(hboost::variant<T...>*) will always return NULL"
    );

    return polymorphic_relaxed_get<U>(operand);
}

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_pointer<const U>::type
polymorphic_strict_get(
      const hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >* operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    ) HBOOST_NOEXCEPT
{
    HBOOST_STATIC_ASSERT_MSG(
        (hboost::detail::variant::holds_element_polymorphic<hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >, U >::value),
        "hboost::variant does not contain specified type U, "
        "call to hboost::polymorphic_get<U>(const hboost::variant<T...>*) will always return NULL"
    );

    return polymorphic_relaxed_get<U>(operand);
}

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_reference<U>::type
polymorphic_strict_get(
      hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >& operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    )
{
    HBOOST_STATIC_ASSERT_MSG(
        (hboost::detail::variant::holds_element_polymorphic<hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >, U >::value),
        "hboost::variant does not contain specified type U, "
        "call to hboost::polymorphic_get<U>(hboost::variant<T...>&) will always throw hboost::bad_polymorphic_get exception"
    );

    return polymorphic_relaxed_get<U>(operand);
}

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_reference<const U>::type
polymorphic_strict_get(
      const hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >& operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    )
{
    HBOOST_STATIC_ASSERT_MSG(
        (hboost::detail::variant::holds_element_polymorphic<hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >, U >::value),
        "hboost::variant does not contain specified type U, "
        "call to hboost::polymorphic_get<U>(const hboost::variant<T...>&) will always throw hboost::bad_polymorphic_get exception"
    );

    return polymorphic_relaxed_get<U>(operand);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// polymorphic_get<U>(variant) methods
//

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_pointer<U>::type
polymorphic_get(
      hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >* operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    ) HBOOST_NOEXCEPT
{
#ifdef HBOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT
    return polymorphic_relaxed_get<U>(operand);
#else
    return polymorphic_strict_get<U>(operand);
#endif

}

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_pointer<const U>::type
polymorphic_get(
      const hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >* operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    ) HBOOST_NOEXCEPT
{
#ifdef HBOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT
    return polymorphic_relaxed_get<U>(operand);
#else
    return polymorphic_strict_get<U>(operand);
#endif
}

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_reference<U>::type
polymorphic_get(
      hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >& operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    )
{
#ifdef HBOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT
    return polymorphic_relaxed_get<U>(operand);
#else
    return polymorphic_strict_get<U>(operand);
#endif
}

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_reference<const U>::type
polymorphic_get(
      const hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >& operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    )
{
#ifdef HBOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT
    return polymorphic_relaxed_get<U>(operand);
#else
    return polymorphic_strict_get<U>(operand);
#endif
}
} // namespace hboost

#endif // HBOOST_VARIANT_POLYMORPHIC_GET_HPP
