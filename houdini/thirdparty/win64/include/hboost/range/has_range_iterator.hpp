// Boost.Range library
//
//  Copyright Neil Groves 2010. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//
// Acknowledgments:
// Ticket #8341: Arno Schoedl - improved handling of has_range_iterator upon
// use-cases where T was const.
#ifndef HBOOST_RANGE_HAS_ITERATOR_HPP_INCLUDED
#define HBOOST_RANGE_HAS_ITERATOR_HPP_INCLUDED

#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/has_xxx.hpp>
#include <hboost/range/iterator.hpp>
#include <hboost/type_traits/remove_reference.hpp>
#include <hboost/utility/enable_if.hpp>

namespace hboost
{
    namespace range_detail
    {
        HBOOST_MPL_HAS_XXX_TRAIT_DEF(type)

        template<class T, class Enabler = void>
        struct has_range_iterator_impl
            : hboost::mpl::false_
        {
        };

        template<class T>
        struct has_range_iterator_impl<
            T,
            HBOOST_DEDUCED_TYPENAME ::hboost::enable_if<
                HBOOST_DEDUCED_TYPENAME mpl::eval_if<is_const<T>,
                    has_type<range_const_iterator<
                                HBOOST_DEDUCED_TYPENAME remove_const<T>::type> >,
                    has_type<range_mutable_iterator<T> >
                >::type
            >::type
        >
            : hboost::mpl::true_
        {
        };

        template<class T, class Enabler = void>
        struct has_range_const_iterator_impl
            : hboost::mpl::false_
        {
        };

        template<class T>
        struct has_range_const_iterator_impl<
            T,
            HBOOST_DEDUCED_TYPENAME ::hboost::enable_if<
                has_type<range_const_iterator<T> >
            >::type
        >
            : hboost::mpl::true_
        {
        };

    } // namespace range_detail

    template<class T>
    struct has_range_iterator
        : range_detail::has_range_iterator_impl<
            HBOOST_DEDUCED_TYPENAME remove_reference<T>::type>
    {};

    template<class T>
    struct has_range_const_iterator
        : range_detail::has_range_const_iterator_impl<
            HBOOST_DEDUCED_TYPENAME remove_reference<T>::type>
    {};
} // namespace hboost

#endif // include guard

