//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_BINARY_SEARCH_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_BINARY_SEARCH_HPP_INCLUDED

#include <hboost/concept_check.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/concepts.hpp>
#include <algorithm>

namespace hboost
{
    namespace range
    {

/// \brief template function binary_search
///
/// range-based version of the binary_search std algorithm
///
/// \pre ForwardRange is a model of the ForwardRangeConcept
/// \pre BinaryPredicate is a model of the BinaryPredicateConcept
template<class ForwardRange, class Value>
inline bool binary_search(const ForwardRange& rng, const Value& val)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<const ForwardRange> ));
    return std::binary_search(hboost::begin(rng), hboost::end(rng), val);
}

/// \overload
template<class ForwardRange, class Value, class BinaryPredicate>
inline bool binary_search(const ForwardRange& rng, const Value& val,
                          BinaryPredicate pred)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<const ForwardRange> ));
    return std::binary_search(hboost::begin(rng), hboost::end(rng), val, pred);
}

    } // namespace range
    using range::binary_search;
} // namespace hboost

#endif // include guard
