//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_REVERSE_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_REVERSE_HPP_INCLUDED

#include <hboost/concept_check.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/concepts.hpp>
#include <hboost/range/detail/range_return.hpp>
#include <algorithm>

namespace hboost
{
    namespace range
    {

/// \brief template function reverse
///
/// range-based version of the reverse std algorithm
///
/// \pre BidirectionalRange is a model of the BidirectionalRangeConcept
template<class BidirectionalRange>
inline BidirectionalRange& reverse(BidirectionalRange& rng)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( BidirectionalRangeConcept<BidirectionalRange> ));
    std::reverse(hboost::begin(rng), hboost::end(rng));
    return rng;
}

/// \overload
template<class BidirectionalRange>
inline const BidirectionalRange& reverse(const BidirectionalRange& rng)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( BidirectionalRangeConcept<const BidirectionalRange> ));
    std::reverse(hboost::begin(rng), hboost::end(rng));
    return rng;
}

    } // namespace range
    using range::reverse;
} // namespace hboost

#endif // include guard
