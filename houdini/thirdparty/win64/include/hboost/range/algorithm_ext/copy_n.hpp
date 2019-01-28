//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_COPY_N_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_COPY_N_HPP_INCLUDED

#include <hboost/assert.hpp>
#include <hboost/concept_check.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/concepts.hpp>
#include <hboost/range/distance.hpp>
#include <hboost/range/iterator.hpp>
#include <hboost/range/iterator_range.hpp>
#include <algorithm>

namespace hboost
{
    namespace range
    {

/// \brief template function copy
///
/// range-based version of the copy std algorithm
///
/// \pre SinglePassRange is a model of the SinglePassRangeConcept
/// \pre OutputIterator is a model of the OutputIteratorConcept
/// \pre 0 <= n <= distance(rng)
template< class SinglePassRange, class Size, class OutputIterator >
inline OutputIterator copy_n(const SinglePassRange& rng, Size n, OutputIterator out)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange> ));
    HBOOST_ASSERT( n <= static_cast<Size>(::hboost::distance(rng)) );
    HBOOST_ASSERT( n >= static_cast<Size>(0) );

    HBOOST_DEDUCED_TYPENAME range_iterator<const SinglePassRange>::type source = ::hboost::begin(rng);

    for (Size i = 0; i < n; ++i, ++out, ++source)
        *out = *source;

    return out;
}

    } // namespace range
    using ::hboost::range::copy_n;
} // namespace hboost

#endif // include guard
