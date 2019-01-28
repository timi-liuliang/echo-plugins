/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_HBOOST_TUPLE_ITERATOR_09262006_1851)
#define FUSION_HBOOST_TUPLE_ITERATOR_09262006_1851

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/iterator/iterator_facade.hpp>
#include <hboost/type_traits/is_const.hpp>
#include <hboost/type_traits/add_const.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/or.hpp>
#include <hboost/mpl/plus.hpp>
#include <hboost/mpl/int.hpp>
#include <hboost/mpl/apply.hpp>
#include <hboost/tuple/tuple.hpp>

namespace hboost { namespace fusion
{
    struct forward_traversal_tag;

    namespace detail
    {
        template <typename T>
        struct hboost_tuple_is_empty : mpl::false_ {};

        template <>
        struct hboost_tuple_is_empty<tuples::null_type> : mpl::true_ {};

        template <>
        struct hboost_tuple_is_empty<tuples::null_type const> : mpl::true_ {};

        template <>
        struct hboost_tuple_is_empty<tuples::tuple<> > : mpl::true_ {};

        template <>
        struct hboost_tuple_is_empty<tuples::tuple<> const> : mpl::true_ {};
    }

    template <typename Cons>
    struct hboost_tuple_iterator_identity;

    template <typename Cons = tuples::null_type>
    struct hboost_tuple_iterator
        : iterator_facade<hboost_tuple_iterator<Cons>, forward_traversal_tag>
    {
        typedef Cons cons_type;

        typedef hboost_tuple_iterator_identity<
            typename add_const<Cons>::type> identity;

        HBOOST_FUSION_GPU_ENABLED
        explicit hboost_tuple_iterator(Cons& in_cons)
            : cons(in_cons) {}
        Cons& cons;

        template <typename Iterator>
        struct value_of : mpl::identity<typename Iterator::cons_type::head_type> {};

        template <typename Iterator>
        struct deref
        {
            typedef typename value_of<Iterator>::type element;

            typedef typename
                mpl::if_<
                    is_const<typename Iterator::cons_type>
                  , typename tuples::access_traits<element>::const_type
                  , typename tuples::access_traits<element>::non_const_type
                >::type
            type;

            HBOOST_FUSION_GPU_ENABLED
            static type
            call(Iterator const& iter)
            {
                return iter.cons.get_head();
            }
        };

        template <typename Iterator>
        struct next
        {
            typedef typename Iterator::cons_type cons_type;
            typedef typename cons_type::tail_type tail_type;

            typedef hboost_tuple_iterator<
                typename mpl::eval_if<
                    is_const<cons_type>
                  , add_const<tail_type>
                  , mpl::identity<tail_type>
                >::type>
            type;

            HBOOST_FUSION_GPU_ENABLED
            static type
            call(Iterator const& iter)
            {
                return type(iter.cons.get_tail());
            }
        };

        template <typename I1, typename I2>
        struct distance;

        // detail
        template <typename I1, typename I2>
        struct lazy_next_distance
        {
            typedef
                typename mpl::plus<
                    mpl::int_<1>,
                    typename distance<
                        typename next<I1>::type,
                        I2
                    >::type
                >::type type;
        };

        template <typename I1, typename I2>
        struct distance
        {
            typedef typename mpl::eval_if<
                hboost::is_same<I1, I2>,
                mpl::int_<0>,
                lazy_next_distance<I1, I2>
            >::type type;

            HBOOST_FUSION_GPU_ENABLED
            static type
            call(I1 const&, I2 const&)
            {
                return type();
            }
        };

        template <typename I1, typename I2>
        struct equal_to
            : is_same<typename I1::identity, typename I2::identity>
        {};

    private:
        // silence MSVC warning C4512: assignment operator could not be generated
        hboost_tuple_iterator& operator= (hboost_tuple_iterator const&);
    };

    template <typename Null>
    struct hboost_tuple_null_iterator
        : iterator_facade<hboost_tuple_iterator<Null>, forward_traversal_tag>
    {
        typedef Null cons_type;

        typedef hboost_tuple_iterator_identity<
            typename add_const<Null>::type> identity;

        template <typename I1, typename I2>
        struct equal_to
            : mpl::or_<
                is_same<I1, I2>
              , mpl::and_<
                    detail::hboost_tuple_is_empty<typename I1::cons_type>
                  , detail::hboost_tuple_is_empty<typename I2::cons_type>
                >
            >
        {};
    };

    template <>
    struct hboost_tuple_iterator<tuples::null_type>
        : hboost_tuple_null_iterator<tuples::null_type>
    {
        template <typename Cons>
        HBOOST_FUSION_GPU_ENABLED
        explicit hboost_tuple_iterator(Cons const&) {}
    };

    template <>
    struct hboost_tuple_iterator<tuples::null_type const>
        : hboost_tuple_null_iterator<tuples::null_type const>
    {
        template <typename Cons>
        HBOOST_FUSION_GPU_ENABLED
        explicit hboost_tuple_iterator(Cons const&) {}
    };

    template <>
    struct hboost_tuple_iterator<tuples::tuple<> >
        : hboost_tuple_null_iterator<tuples::tuple<> >
    {
        template <typename Cons>
        HBOOST_FUSION_GPU_ENABLED
        explicit hboost_tuple_iterator(Cons const&) {}
    };

    template <>
    struct hboost_tuple_iterator<tuples::tuple<> const>
        : hboost_tuple_null_iterator<tuples::tuple<> const>
    {
        template <typename Cons>
        HBOOST_FUSION_GPU_ENABLED
        explicit hboost_tuple_iterator(Cons const&) {}
    };
}}

#ifdef HBOOST_FUSION_WORKAROUND_FOR_LWG_2408
namespace std
{
    template <typename Cons>
    struct iterator_traits< ::hboost::fusion::hboost_tuple_iterator<Cons> >
    { };
}
#endif

#endif


