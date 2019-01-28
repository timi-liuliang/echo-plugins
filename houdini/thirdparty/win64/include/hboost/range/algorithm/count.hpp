//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_COUNT_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_COUNT_HPP_INCLUDED

#include <hboost/concept_check.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/concepts.hpp>
#include <hboost/range/difference_type.hpp>
#include <algorithm>

namespace hboost
{
    namespace range
    {

/// \brief template function count
///
/// range-based version of the count std algorithm
///
/// \pre SinglePassRange is a model of the SinglePassRangeConcept
template< class SinglePassRange, class Value >
inline HBOOST_DEDUCED_TYPENAME range_difference<SinglePassRange>::type
count(SinglePassRange& rng, const Value& val)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<SinglePassRange> ));
    return std::count(hboost::begin(rng), hboost::end(rng), val);
}

/// \overload
template< class SinglePassRange, class Value >
inline HBOOST_DEDUCED_TYPENAME range_difference<SinglePassRange const>::type
count(const SinglePassRange& rng, const Value& val)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange> ));
    return std::count(hboost::begin(rng), hboost::end(rng), val);
}

    } // namespace range
    using range::count;
} // namespace hboost

#endif // include guard
