//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_EQUAL_RANGE_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_EQUAL_RANGE_HPP_INCLUDED

#include <hboost/concept_check.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/concepts.hpp>
#include <algorithm>

namespace hboost
{
    namespace range
    {

/// \brief template function equal_range
///
/// range-based version of the equal_range std algorithm
///
/// \pre ForwardRange is a model of the ForwardRangeConcept
/// \pre SortPredicate is a model of the BinaryPredicateConcept
template<class ForwardRange, class Value>
inline std::pair<
        HBOOST_DEDUCED_TYPENAME hboost::range_iterator<ForwardRange>::type,
        HBOOST_DEDUCED_TYPENAME hboost::range_iterator<ForwardRange>::type
       >
equal_range(ForwardRange& rng, const Value& val)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<ForwardRange> ));
    return std::equal_range(hboost::begin(rng), hboost::end(rng), val);
}

/// \overload
template<class ForwardRange, class Value>
inline std::pair<
        HBOOST_DEDUCED_TYPENAME hboost::range_iterator<const ForwardRange>::type,
        HBOOST_DEDUCED_TYPENAME hboost::range_iterator<const ForwardRange>::type
       >
equal_range(const ForwardRange& rng, const Value& val)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<const ForwardRange> ));
    return std::equal_range(hboost::begin(rng), hboost::end(rng), val);
}

/// \overload
template<class ForwardRange, class Value, class SortPredicate>
inline std::pair<
        HBOOST_DEDUCED_TYPENAME hboost::range_iterator<ForwardRange>::type,
        HBOOST_DEDUCED_TYPENAME hboost::range_iterator<ForwardRange>::type
       >
equal_range(ForwardRange& rng, const Value& val, SortPredicate pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<ForwardRange> ));
    return std::equal_range(hboost::begin(rng), hboost::end(rng), val, pred);
}

/// \overload
template<class ForwardRange, class Value, class SortPredicate>
inline std::pair<
        HBOOST_DEDUCED_TYPENAME hboost::range_iterator<const ForwardRange>::type,
        HBOOST_DEDUCED_TYPENAME hboost::range_iterator<const ForwardRange>::type
       >
equal_range(const ForwardRange& rng, const Value& val, SortPredicate pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<const ForwardRange> ));
    return std::equal_range(hboost::begin(rng), hboost::end(rng), val, pred);
}

    } // namespace range
    using range::equal_range;
} // namespace hboost

#endif // include guard
