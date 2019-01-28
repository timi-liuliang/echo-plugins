//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_REMOVE_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_REMOVE_HPP_INCLUDED

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

/// \brief template function remove
///
/// range-based version of the remove std algorithm
///
/// \pre ForwardRange is a model of the ForwardRangeConcept
template< class ForwardRange, class Value >
inline HBOOST_DEDUCED_TYPENAME range_iterator<ForwardRange>::type
remove(ForwardRange& rng, const Value& val)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<ForwardRange> ));
    return std::remove(hboost::begin(rng),hboost::end(rng),val);
}

/// \overload
template< class ForwardRange, class Value >
inline HBOOST_DEDUCED_TYPENAME range_iterator<const ForwardRange>::type
remove(const ForwardRange& rng, const Value& val)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<const ForwardRange> ));
    return std::remove(hboost::begin(rng),hboost::end(rng),val);
}

// range_return overloads

/// \overload
template< range_return_value re, class ForwardRange, class Value >
inline HBOOST_DEDUCED_TYPENAME range_return<ForwardRange,re>::type
remove(ForwardRange& rng, const Value& val)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<ForwardRange> ));
    return range_return<ForwardRange,re>::pack(
        std::remove(hboost::begin(rng), hboost::end(rng), val),
        rng);
}

/// \overload
template< range_return_value re, class ForwardRange, class Value >
inline HBOOST_DEDUCED_TYPENAME range_return<const ForwardRange,re>::type
remove(const ForwardRange& rng, const Value& val)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<const ForwardRange> ));
    return range_return<const ForwardRange,re>::pack(
        std::remove(hboost::begin(rng), hboost::end(rng), val),
        rng);
}

    } // namespace range
    using range::remove;
} // namespace hboost

#endif // include guard
