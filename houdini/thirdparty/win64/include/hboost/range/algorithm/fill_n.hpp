//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_FILL_N_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_FILL_N_HPP_INCLUDED

#include <hboost/assert.hpp>
#include <hboost/concept_check.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/concepts.hpp>
#include <algorithm>

namespace hboost
{
    namespace range
    {

/// \brief template function fill_n
///
/// range-based version of the fill_n std algorithm
///
/// \pre ForwardRange is a model of the ForwardRangeConcept
/// \pre n <= std::distance(hboost::begin(rng), hboost::end(rng))
template< class ForwardRange, class Size, class Value >
inline ForwardRange& fill_n(ForwardRange& rng, Size n, const Value& val)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<ForwardRange> ));
    HBOOST_ASSERT( static_cast<Size>(std::distance(hboost::begin(rng), hboost::end(rng))) >= n );
    std::fill_n(hboost::begin(rng), n, val);
    return rng;
}

/// \overload
template< class ForwardRange, class Size, class Value >
inline const ForwardRange& fill_n(const ForwardRange& rng, Size n, const Value& val)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<const ForwardRange> ));
    HBOOST_ASSERT( static_cast<Size>(std::distance(hboost::begin(rng), hboost::end(rng))) >= n );
    std::fill_n(hboost::begin(rng), n, val);
    return rng;
}

    } // namespace range
    using range::fill_n;
} // namespace hboost

#endif // include guard
