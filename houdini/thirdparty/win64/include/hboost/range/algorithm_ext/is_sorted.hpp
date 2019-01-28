//  Copyright Bryce Lelbach 2010
//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_EXT_IS_SORTED_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_EXT_IS_SORTED_HPP_INCLUDED

#include <hboost/concept_check.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/concepts.hpp>
#include <hboost/range/value_type.hpp>
#include <hboost/detail/is_sorted.hpp>
#include <algorithm>

namespace hboost
{
    namespace range
    {

/// \brief template function is_sorted
///
/// range-based version of the is_sorted std algorithm
///
/// \pre SinglePassRange is a model of the SinglePassRangeConcept
template<class SinglePassRange>
inline bool is_sorted(const SinglePassRange& rng)
{
    HBOOST_RANGE_CONCEPT_ASSERT((SinglePassRangeConcept<const SinglePassRange>));
    HBOOST_RANGE_CONCEPT_ASSERT((LessThanComparableConcept<HBOOST_DEDUCED_TYPENAME
      range_value<const SinglePassRange>::type>));
    return ::hboost::detail::is_sorted(hboost::begin(rng), hboost::end(rng));
}

/// \overload
template<class SinglePassRange, class BinaryPredicate>
inline bool is_sorted(const SinglePassRange& rng, BinaryPredicate pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT((SinglePassRangeConcept<const SinglePassRange>));
    HBOOST_RANGE_CONCEPT_ASSERT((BinaryPredicateConcept<BinaryPredicate,
      HBOOST_DEDUCED_TYPENAME range_value<const SinglePassRange>::type,
      HBOOST_DEDUCED_TYPENAME range_value<const SinglePassRange>::type>));
    return ::hboost::detail::is_sorted(hboost::begin(rng), hboost::end(rng), pred);
}

    } // namespace range

using range::is_sorted;

} // namespace hboost

#endif // include guard
