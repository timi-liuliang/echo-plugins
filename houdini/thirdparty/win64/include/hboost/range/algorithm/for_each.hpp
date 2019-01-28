//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_FOR_EACH_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_FOR_EACH_HPP_INCLUDED

#include <hboost/concept_check.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/concepts.hpp>
#include <hboost/utility/enable_if.hpp>
#include <hboost/ref.hpp>
#include <algorithm>

#if HBOOST_WORKAROUND(HBOOST_MSVC, == 1600)
#include <xutility>
#endif

namespace hboost
{
    namespace range
    {

#if HBOOST_WORKAROUND(HBOOST_MSVC, == 1600)
        namespace for_each_detail
        {
            template<typename Iterator, typename UnaryFunction>
            inline UnaryFunction
            for_each_impl(Iterator first, Iterator last, UnaryFunction fun,
                          typename ::hboost::enable_if<
                            is_reference_wrapper<UnaryFunction>,
                            void
                          >::type* = 0)
            {
                    typedef typename std::_Get_unchecked_type<Iterator>::type
                                unchecked_iterator;

                    unchecked_iterator unchecked_last = std::_Unchecked(last);
                    for (unchecked_iterator unchecked_first = std::_Unchecked(first); first != last; ++first)
                            fun.get()(*unchecked_first);

                    return fun;
            }

            template<typename Iterator, typename UnaryFunction>
            inline UnaryFunction
            for_each_impl(Iterator first, Iterator last, UnaryFunction fn,
                          typename disable_if<
                            is_reference_wrapper<UnaryFunction>,
                            void
                          >::type* = 0)
            {
                return std::for_each<Iterator, UnaryFunction>(first, last, fn);
            }
        }
#endif

/// \brief template function for_each
///
/// range-based version of the for_each std algorithm
///
/// \pre SinglePassRange is a model of the SinglePassRangeConcept
/// \pre UnaryFunction is a model of the UnaryFunctionConcept
template< class SinglePassRange, class UnaryFunction >
inline UnaryFunction for_each(SinglePassRange & rng, UnaryFunction fun)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<SinglePassRange> ));
    
#if HBOOST_WORKAROUND(HBOOST_MSVC, == 1600)
        return for_each_detail::for_each_impl<
                typename range_iterator<SinglePassRange>::type,
                UnaryFunction
        >(hboost::begin(rng), hboost::end(rng), fun);
#else
    return std::for_each<
        HBOOST_DEDUCED_TYPENAME range_iterator<SinglePassRange>::type,
        UnaryFunction
    >(hboost::begin(rng),hboost::end(rng),fun);
#endif    
}

/// \overload
template< class SinglePassRange, class UnaryFunction >
inline UnaryFunction for_each(const SinglePassRange& rng, UnaryFunction fun)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange> ));
    
#if HBOOST_WORKAROUND(HBOOST_MSVC, == 1600)
        return for_each_detail::for_each_impl<
                typename range_iterator<const SinglePassRange>::type,
                UnaryFunction
        >(hboost::begin(rng), hboost::end(rng), fun);
#else    
    return std::for_each<
        HBOOST_DEDUCED_TYPENAME range_iterator<const SinglePassRange>::type,
        UnaryFunction
    >(hboost::begin(rng), hboost::end(rng), fun);
#endif    
}

    } // namespace range
    using range::for_each;
} // namespace hboost

#endif // include guard
