//-----------------------------------------------------------------------------
// boost variant/detail/element_index.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2014-2015 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_VARIANT_DETAIL_ELEMENT_INDEX_HPP
#define HBOOST_VARIANT_DETAIL_ELEMENT_INDEX_HPP

#include "hboost/config.hpp"
#include "hboost/variant/recursive_wrapper_fwd.hpp"
#include "hboost/variant/variant_fwd.hpp"

#include "hboost/type_traits/remove_cv.hpp"
#include "hboost/type_traits/remove_reference.hpp"
#include "hboost/mpl/find_if.hpp"

namespace hboost { namespace detail { namespace variant {

template <class VariantElement, class T>
struct variant_element_functor :
    hboost::mpl::or_<
        hboost::is_same<VariantElement, T>,
        hboost::is_same<VariantElement, hboost::recursive_wrapper<T> >,
        hboost::is_same<VariantElement, T& >
    >
{};

template <class Types, class T>
struct element_iterator_impl :
    hboost::mpl::find_if<
        Types,
        hboost::mpl::or_<
            variant_element_functor<hboost::mpl::_1, T>,
            variant_element_functor<hboost::mpl::_1, typename hboost::remove_cv<T>::type >
        >
    >
{};

template <class Variant, class T>
struct element_iterator :
    element_iterator_impl< typename Variant::types, typename hboost::remove_reference<T>::type >
{};

template <class Variant, class T>
struct holds_element :
    hboost::mpl::not_<
        hboost::is_same<
            typename hboost::mpl::end<typename Variant::types>::type,
            typename element_iterator<Variant, T>::type
        >
    >
{};


}}} // namespace hboost::detail::variant

#endif // HBOOST_VARIANT_DETAIL_ELEMENT_INDEX_HPP
