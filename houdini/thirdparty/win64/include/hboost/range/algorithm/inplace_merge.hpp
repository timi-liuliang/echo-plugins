//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_INPLACE_MERGE_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_INPLACE_MERGE_HPP_INCLUDED

#include <hboost/concept_check.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/concepts.hpp>
#include <algorithm>

namespace hboost
{
    namespace range
    {

/// \brief template function inplace_merge
///
/// range-based version of the inplace_merge std algorithm
///
/// \pre BidirectionalRange is a model of the BidirectionalRangeConcept
/// \pre BinaryPredicate is a model of the BinaryPredicateConcept
template<class BidirectionalRange>
inline BidirectionalRange& inplace_merge(BidirectionalRange& rng,
    HBOOST_DEDUCED_TYPENAME range_iterator<BidirectionalRange>::type middle)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( BidirectionalRangeConcept<BidirectionalRange> ));
    std::inplace_merge(hboost::begin(rng), middle, hboost::end(rng));
    return rng;
}

/// \overload
template<class BidirectionalRange>
inline const BidirectionalRange& inplace_merge(const BidirectionalRange& rng,
    HBOOST_DEDUCED_TYPENAME hboost::range_iterator<const BidirectionalRange>::type middle)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( BidirectionalRangeConcept<const BidirectionalRange> ));
    std::inplace_merge(hboost::begin(rng), middle, hboost::end(rng));
    return rng;
}

/// \overload
template<class BidirectionalRange, class BinaryPredicate>
inline BidirectionalRange& inplace_merge(BidirectionalRange& rng,
    HBOOST_DEDUCED_TYPENAME hboost::range_iterator<BidirectionalRange>::type middle,
    BinaryPredicate pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( BidirectionalRangeConcept<BidirectionalRange> ));
    std::inplace_merge(hboost::begin(rng), middle, hboost::end(rng), pred);
    return rng;
}

/// \overload
template<class BidirectionalRange, class BinaryPredicate>
inline const BidirectionalRange& inplace_merge(const BidirectionalRange& rng,
    HBOOST_DEDUCED_TYPENAME hboost::range_iterator<const BidirectionalRange>::type middle,
    BinaryPredicate pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( BidirectionalRangeConcept<const BidirectionalRange> ));
    std::inplace_merge(hboost::begin(rng), middle, hboost::end(rng), pred);
    return rng;
}

    } // namespace range
    using range::inplace_merge;
} // namespace hboost

#endif // include guard
