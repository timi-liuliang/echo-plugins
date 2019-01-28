//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_PARTIAL_SORT_COPY_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_PARTIAL_SORT_COPY_HPP_INCLUDED

#include <hboost/concept_check.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/concepts.hpp>
#include <hboost/range/value_type.hpp>
#include <algorithm>

namespace hboost
{
    namespace range
    {

/// \brief template function partial_sort_copy
///
/// range-based version of the partial_sort_copy std algorithm
///
/// \pre SinglePassRange is a model of the SinglePassRangeConcept
/// \pre RandomAccessRange is a model of the Mutable_RandomAccessRangeConcept
/// \pre BinaryPredicate is a model of the BinaryPredicateConcept
template<class SinglePassRange, class RandomAccessRange>
inline HBOOST_DEDUCED_TYPENAME range_iterator<RandomAccessRange>::type
partial_sort_copy(const SinglePassRange& rng1, RandomAccessRange& rng2)
{
    HBOOST_RANGE_CONCEPT_ASSERT((SinglePassRangeConcept<const SinglePassRange>));

    return std::partial_sort_copy(hboost::begin(rng1), hboost::end(rng1),
        hboost::begin(rng2), hboost::end(rng2));
}

/// \overload
template<class SinglePassRange, class RandomAccessRange>
inline HBOOST_DEDUCED_TYPENAME range_iterator<RandomAccessRange>::type
partial_sort_copy(const SinglePassRange& rng1, const RandomAccessRange& rng2)
{
    HBOOST_RANGE_CONCEPT_ASSERT((SinglePassRangeConcept<const SinglePassRange>));

    return std::partial_sort_copy(hboost::begin(rng1), hboost::end(rng1),
        hboost::begin(rng2), hboost::end(rng2));
}

/// \overload
template<class SinglePassRange, class RandomAccessRange,
         class BinaryPredicate>
inline HBOOST_DEDUCED_TYPENAME range_iterator<RandomAccessRange>::type
partial_sort_copy(const SinglePassRange& rng1, RandomAccessRange& rng2,
    BinaryPredicate pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT((SinglePassRangeConcept<const SinglePassRange>));

    return std::partial_sort_copy(hboost::begin(rng1), hboost::end(rng1),
        hboost::begin(rng2), hboost::end(rng2), pred);
}

/// \overload
template<class SinglePassRange, class RandomAccessRange,
         class BinaryPredicate>
inline HBOOST_DEDUCED_TYPENAME range_iterator<const RandomAccessRange>::type
partial_sort_copy(const SinglePassRange& rng1, const RandomAccessRange& rng2,
    BinaryPredicate pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT((SinglePassRangeConcept<const SinglePassRange>));

    return std::partial_sort_copy(hboost::begin(rng1), hboost::end(rng1),
        hboost::begin(rng2), hboost::end(rng2), pred);
}

    } // namespace range
    using range::partial_sort_copy;
} // namespace hboost

#endif // include guard
