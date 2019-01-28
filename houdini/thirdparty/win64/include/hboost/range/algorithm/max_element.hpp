//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_MAX_ELEMENT_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_MAX_ELEMENT_HPP_INCLUDED

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

/// \brief template function max_element
///
/// range-based version of the max_element std algorithm
///
/// \pre ForwardRange is a model of the ForwardRangeConcept
/// \pre BinaryPredicate is a model of the BinaryPredicateConcept
template<class ForwardRange>
inline HBOOST_DEDUCED_TYPENAME range_iterator<ForwardRange>::type
max_element(ForwardRange& rng)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<ForwardRange> ));
    return std::max_element(hboost::begin(rng), hboost::end(rng));
}

/// \overload
template<class ForwardRange>
inline HBOOST_DEDUCED_TYPENAME range_iterator<const ForwardRange>::type
max_element(const ForwardRange& rng)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<const ForwardRange> ));
    return std::max_element(hboost::begin(rng), hboost::end(rng));
}

/// \overload
template<class ForwardRange, class BinaryPredicate>
inline HBOOST_DEDUCED_TYPENAME range_iterator<ForwardRange>::type
max_element(ForwardRange& rng, BinaryPredicate pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<ForwardRange> ));
    return std::max_element(hboost::begin(rng), hboost::end(rng), pred);
}

/// \overload
template<class ForwardRange, class BinaryPredicate>
inline HBOOST_DEDUCED_TYPENAME range_iterator<const ForwardRange>::type
max_element(const ForwardRange& rng, BinaryPredicate pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<const ForwardRange> ));
    return std::max_element(hboost::begin(rng), hboost::end(rng), pred);
}

// range_return overloads

/// \overload
template<range_return_value re, class ForwardRange>
inline HBOOST_DEDUCED_TYPENAME range_return<ForwardRange,re>::type
max_element(ForwardRange& rng)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<ForwardRange> ));
    return range_return<ForwardRange,re>::pack(
        std::max_element(hboost::begin(rng), hboost::end(rng)),
        rng);
}

/// \overload
template<range_return_value re, class ForwardRange>
inline HBOOST_DEDUCED_TYPENAME range_return<const ForwardRange,re>::type
max_element(const ForwardRange& rng)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<const ForwardRange> ));
    return range_return<const ForwardRange,re>::pack(
        std::max_element(hboost::begin(rng), hboost::end(rng)),
        rng);
}

/// \overload
template<range_return_value re, class ForwardRange, class BinaryPredicate>
inline HBOOST_DEDUCED_TYPENAME range_return<ForwardRange,re>::type
max_element(ForwardRange& rng, BinaryPredicate pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<ForwardRange> ));
    return range_return<ForwardRange,re>::pack(
        std::max_element(hboost::begin(rng), hboost::end(rng), pred),
        rng);
}

/// \overload
template<range_return_value re, class ForwardRange, class BinaryPredicate>
inline HBOOST_DEDUCED_TYPENAME range_return<const ForwardRange,re>::type
max_element(const ForwardRange& rng, BinaryPredicate pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<const ForwardRange> ));
    return range_return<const ForwardRange,re>::pack(
        std::max_element(hboost::begin(rng), hboost::end(rng), pred),
        rng);
}

    } // namespace range
    using range::max_element;
} // namespace hboost

#endif // include guard
