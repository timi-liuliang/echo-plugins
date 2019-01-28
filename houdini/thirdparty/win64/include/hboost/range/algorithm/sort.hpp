//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_SORT_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_SORT_HPP_INCLUDED

#include <hboost/concept_check.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/concepts.hpp>
#include <algorithm>

namespace hboost
{
    namespace range
    {

/// \brief template function sort
///
/// range-based version of the sort std algorithm
///
/// \pre RandomAccessRange is a model of the RandomAccessRangeConcept
/// \pre BinaryPredicate is a model of the BinaryPredicateConcept
template<class RandomAccessRange>
inline RandomAccessRange& sort(RandomAccessRange& rng)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( RandomAccessRangeConcept<RandomAccessRange> ));
    std::sort(hboost::begin(rng), hboost::end(rng));
    return rng;
}

/// \overload
template<class RandomAccessRange>
inline const RandomAccessRange& sort(const RandomAccessRange& rng)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( RandomAccessRangeConcept<const RandomAccessRange> ));
    std::sort(hboost::begin(rng), hboost::end(rng));
    return rng;
}

/// \overload
template<class RandomAccessRange, class BinaryPredicate>
inline RandomAccessRange& sort(RandomAccessRange& rng, BinaryPredicate pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( RandomAccessRangeConcept<RandomAccessRange> ));
    std::sort(hboost::begin(rng), hboost::end(rng), pred);
    return rng;
}

/// \overload
template<class RandomAccessRange, class BinaryPredicate>
inline const RandomAccessRange& sort(const RandomAccessRange& rng, BinaryPredicate pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( RandomAccessRangeConcept<const RandomAccessRange> ));
    std::sort(hboost::begin(rng), hboost::end(rng), pred);
    return rng;
}

    } // namespace range
    using range::sort;
} // namespace hboost

#endif // include guard
