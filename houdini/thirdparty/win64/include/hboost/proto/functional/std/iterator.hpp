///////////////////////////////////////////////////////////////////////////////
/// \file iterator.hpp
/// Proto callables for std functions found in \<iterator\>
//
//  Copyright 2012 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PROTO_FUNCTIONAL_STD_ITERATOR_HPP_EAN_27_08_2012
#define HBOOST_PROTO_FUNCTIONAL_STD_ITERATOR_HPP_EAN_27_08_2012

#include <iterator>
#include <hboost/type_traits/remove_const.hpp>
#include <hboost/type_traits/remove_reference.hpp>
#include <hboost/proto/proto_fwd.hpp>

namespace hboost { namespace proto { namespace functional
{

    // A PolymorphicFunctionObject wrapping std::advance
    struct advance
    {
        HBOOST_PROTO_CALLABLE()

        typedef void result_type;

        template<typename InputIterator, typename Distance>
        void operator()(InputIterator &x, Distance n) const
        {
            std::advance(x, n);
        }
    };

    // A PolymorphicFunctionObject wrapping std::distance
    struct distance
    {
        HBOOST_PROTO_CALLABLE()

        template<typename Sig>
        struct result;

        template<typename This, typename InputIter1, typename InputIter2>
        struct result<This(InputIter1, InputIter2)>
        {
            typedef
                typename std::iterator_traits<
                    typename hboost::remove_const<
                        typename hboost::remove_reference<InputIter1>::type
                    >::type
                >::difference_type
            type;
        };

        template<typename InputIterator>
        typename std::iterator_traits<InputIterator>::difference_type
        operator()(InputIterator first, InputIterator last) const
        {
            return std::distance(first, last);
        }
    };

    // A PolymorphicFunctionObject wrapping std::next
    struct next
    {
        HBOOST_PROTO_CALLABLE()

        template<typename Sig>
        struct result;

        template<typename This, typename ForwardIterator>
        struct result<This(ForwardIterator)>
        {
            typedef
                typename hboost::remove_const<
                    typename hboost::remove_reference<ForwardIterator>::type
                >::type
            type;
        };

        template<typename This, typename ForwardIterator, typename Distance>
        struct result<This(ForwardIterator, Distance)>
        {
            typedef
                typename hboost::remove_const<
                    typename hboost::remove_reference<ForwardIterator>::type
                >::type
            type;
        };

        template<typename ForwardIterator>
        ForwardIterator operator()(ForwardIterator x) const
        {
            return std::advance(
                x
              , static_cast<typename std::iterator_traits<ForwardIterator>::difference_type>(1)
            );
        }

        template<typename ForwardIterator>
        ForwardIterator operator()(
            ForwardIterator x
          , typename std::iterator_traits<ForwardIterator>::difference_type n
        ) const
        {
            return std::advance(x, n);
        }
    };

    // A PolymorphicFunctionObject wrapping std::prior
    struct prior
    {
        HBOOST_PROTO_CALLABLE()

        template<typename Sig>
        struct result;

        template<typename This, typename BidirectionalIterator>
        struct result<This(BidirectionalIterator)>
        {
            typedef
                typename hboost::remove_const<
                    typename hboost::remove_reference<BidirectionalIterator>::type
                >::type
            type;
        };

        template<typename This, typename BidirectionalIterator, typename Distance>
        struct result<This(BidirectionalIterator, Distance)>
        {
            typedef
                typename hboost::remove_const<
                    typename hboost::remove_reference<BidirectionalIterator>::type
                >::type
            type;
        };

        template<typename BidirectionalIterator>
        BidirectionalIterator operator()(BidirectionalIterator x) const
        {
            return std::advance(
                x
              , -static_cast<typename std::iterator_traits<BidirectionalIterator>::difference_type>(1)
            );
        }

        template<typename BidirectionalIterator>
        BidirectionalIterator operator()(
            BidirectionalIterator x
          , typename std::iterator_traits<BidirectionalIterator>::difference_type n
        ) const
        {
            return std::advance(x, -n);
        }
    };

}}}

#endif
