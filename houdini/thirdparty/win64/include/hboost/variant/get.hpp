//-----------------------------------------------------------------------------
// boost variant/get.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003 Eric Friedman, Itay Maman
// Copyright (c) 2014 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_VARIANT_GET_HPP
#define HBOOST_VARIANT_GET_HPP

#include <exception>

#include "hboost/config.hpp"
#include "hboost/detail/workaround.hpp"
#include "hboost/static_assert.hpp"
#include "hboost/throw_exception.hpp"
#include "hboost/utility/addressof.hpp"
#include "hboost/variant/variant_fwd.hpp"
#include "hboost/variant/detail/element_index.hpp"

#include "hboost/type_traits/add_reference.hpp"
#include "hboost/type_traits/add_pointer.hpp"

namespace hboost {

//////////////////////////////////////////////////////////////////////////
// class bad_get
//
// The exception thrown in the event of a failed get of a value.
//
class HBOOST_SYMBOL_VISIBLE bad_get
    : public std::exception
{
public: // std::exception implementation

    virtual const char * what() const HBOOST_NOEXCEPT_OR_NOTHROW
    {
        return "hboost::bad_get: "
               "failed value get using hboost::get";
    }

};

//////////////////////////////////////////////////////////////////////////
// function template get<T>
//
// Retrieves content of given variant object if content is of type T.
// Otherwise: pointer ver. returns 0; reference ver. throws bad_get.
//

namespace detail { namespace variant {

// (detail) class template get_visitor
//
// Generic static visitor that: if the value is of the specified type,
// returns a pointer to the value it visits; else a null pointer.
//
template <typename T>
struct get_visitor
{
private: // private typedefs

    typedef typename add_pointer<T>::type pointer;
    typedef typename add_reference<T>::type reference;

public: // visitor typedefs

    typedef pointer result_type;

public: // visitor interfaces

    pointer operator()(reference operand) const HBOOST_NOEXCEPT
    {
        return hboost::addressof(operand);
    }

    template <typename U>
    pointer operator()(const U&) const HBOOST_NOEXCEPT
    {
        return static_cast<pointer>(0);
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// relaxed_get<U>(variant) methods
//
template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_pointer<U>::type
relaxed_get(
      hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >* operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    ) HBOOST_NOEXCEPT
{
    typedef typename add_pointer<U>::type U_ptr;
    if (!operand) return static_cast<U_ptr>(0);

    detail::variant::get_visitor<U> v;
    return operand->apply_visitor(v);
}

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_pointer<const U>::type
relaxed_get(
      const hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >* operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    ) HBOOST_NOEXCEPT
{
    typedef typename add_pointer<const U>::type U_ptr;
    if (!operand) return static_cast<U_ptr>(0);

    detail::variant::get_visitor<const U> v;
    return operand->apply_visitor(v);
}

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_reference<U>::type
relaxed_get(
      hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >& operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    )
{
    typedef typename add_pointer<U>::type U_ptr;
    U_ptr result = relaxed_get<U>(&operand);

    if (!result)
        hboost::throw_exception(bad_get());
    return *result;
}

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_reference<const U>::type
relaxed_get(
      const hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >& operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    )
{
    typedef typename add_pointer<const U>::type U_ptr;
    U_ptr result = relaxed_get<const U>(&operand);

    if (!result)
        hboost::throw_exception(bad_get());
    return *result;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// strict_get<U>(variant) methods
//
template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_pointer<U>::type
strict_get(
      hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >* operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    ) HBOOST_NOEXCEPT
{
    HBOOST_STATIC_ASSERT_MSG(
        (hboost::detail::variant::holds_element<hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >, U >::value),
        "hboost::variant does not contain specified type U, "
        "call to hboost::get<U>(hboost::variant<T...>*) will always return NULL"
    );

    return relaxed_get<U>(operand);
}

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_pointer<const U>::type
strict_get(
      const hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >* operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    ) HBOOST_NOEXCEPT
{
    HBOOST_STATIC_ASSERT_MSG(
        (hboost::detail::variant::holds_element<hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >, const U >::value),
        "hboost::variant does not contain specified type U, "
        "call to hboost::get<U>(const hboost::variant<T...>*) will always return NULL"
    );

    return relaxed_get<U>(operand);
}

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_reference<U>::type
strict_get(
      hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >& operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    )
{
    HBOOST_STATIC_ASSERT_MSG(
        (hboost::detail::variant::holds_element<hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >, U >::value),
        "hboost::variant does not contain specified type U, "
        "call to hboost::get<U>(hboost::variant<T...>&) will always throw hboost::bad_get exception"
    );

    return relaxed_get<U>(operand);
}

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_reference<const U>::type
strict_get(
      const hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >& operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    )
{
    HBOOST_STATIC_ASSERT_MSG(
        (hboost::detail::variant::holds_element<hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >, const U >::value),
        "hboost::variant does not contain specified type U, "
        "call to hboost::get<U>(const hboost::variant<T...>&) will always throw hboost::bad_get exception"
    );

    return relaxed_get<U>(operand);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get<U>(variant) methods
//

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_pointer<U>::type
get(
      hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >* operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    ) HBOOST_NOEXCEPT
{
#ifdef HBOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT
    return relaxed_get<U>(operand);
#else
    return strict_get<U>(operand);
#endif

}

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_pointer<const U>::type
get(
      const hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >* operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    ) HBOOST_NOEXCEPT
{
#ifdef HBOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT
    return relaxed_get<U>(operand);
#else
    return strict_get<U>(operand);
#endif
}

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_reference<U>::type
get(
      hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >& operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    )
{
#ifdef HBOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT
    return relaxed_get<U>(operand);
#else
    return strict_get<U>(operand);
#endif
}

template <typename U, HBOOST_VARIANT_ENUM_PARAMS(typename T) >
inline
    typename add_reference<const U>::type
get(
      const hboost::variant< HBOOST_VARIANT_ENUM_PARAMS(T) >& operand
      HBOOST_VARIANT_AUX_GET_EXPLICIT_TEMPLATE_TYPE(U)
    )
{
#ifdef HBOOST_VARIANT_USE_RELAXED_GET_BY_DEFAULT
    return relaxed_get<U>(operand);
#else
    return strict_get<U>(operand);
#endif
}

} // namespace hboost

#endif // HBOOST_VARIANT_GET_HPP
