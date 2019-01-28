// Boost.Range library
//
//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_EXT_IOTA_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_EXT_IOTA_HPP_INCLUDED

#include <hboost/range/config.hpp>
#include <hboost/range/concepts.hpp>
#include <hboost/range/iterator.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>

namespace hboost
{
    namespace range
    {

template< class ForwardRange, class Value >
inline ForwardRange& iota( ForwardRange& rng, Value x )
{
    HBOOST_CONCEPT_ASSERT(( ForwardRangeConcept<ForwardRange> ));
    typedef HBOOST_DEDUCED_TYPENAME range_iterator<ForwardRange>::type iterator_t;

    iterator_t last_target = ::hboost::end(rng);
    for (iterator_t target = ::hboost::begin(rng); target != last_target; ++target, ++x)
        *target = x;

    return rng;
}

template< class ForwardRange, class Value >
inline const ForwardRange& iota( const ForwardRange& rng, Value x )
{
    HBOOST_CONCEPT_ASSERT(( ForwardRangeConcept<const ForwardRange> ));
    typedef HBOOST_DEDUCED_TYPENAME range_iterator<const ForwardRange>::type iterator_t;
    
    iterator_t last_target = ::hboost::end(rng);
    for (iterator_t target = ::hboost::begin(rng); target != last_target; ++target, ++x)
        *target = x;
    
    return rng;
}

    } // namespace range
    using range::iota;
} // namespace hboost

#endif // include guard
