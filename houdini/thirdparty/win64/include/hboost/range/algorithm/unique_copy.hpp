//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_UNIQUE_COPY_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_UNIQUE_COPY_HPP_INCLUDED

#include <hboost/concept_check.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/concepts.hpp>
#include <algorithm>

namespace hboost
{
    namespace range
    {

/// \brief template function unique_copy
///
/// range-based version of the unique_copy std algorithm
///
/// \pre SinglePassRange is a model of the SinglePassRangeConcept
/// \pre OutputIterator is a model of the OutputIteratorConcept
/// \pre BinaryPredicate is a model of the BinaryPredicateConcept
template< class SinglePassRange, class OutputIterator >
inline OutputIterator
unique_copy( const SinglePassRange& rng, OutputIterator out_it )
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange> ));
    return std::unique_copy(hboost::begin(rng), hboost::end(rng), out_it);
}
/// \overload
template< class SinglePassRange, class OutputIterator, class BinaryPredicate >
inline OutputIterator
unique_copy( const SinglePassRange& rng, OutputIterator out_it,
             BinaryPredicate pred )
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange> ));
    return std::unique_copy(hboost::begin(rng), hboost::end(rng), out_it, pred);
}

    } // namespace range
    using range::unique_copy;
} // namespace hboost

#endif // include guard
