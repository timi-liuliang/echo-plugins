//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_REPLACE_COPY_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_REPLACE_COPY_HPP_INCLUDED

#include <hboost/concept_check.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/concepts.hpp>
#include <algorithm>

namespace hboost
{
    namespace range
    {

/// \brief template function replace_copy
///
/// range-based version of the replace_copy std algorithm
///
/// \pre ForwardRange is a model of the ForwardRangeConcept
template< class ForwardRange, class OutputIterator, class Value >
inline OutputIterator
replace_copy(const ForwardRange& rng, OutputIterator out_it, const Value& what,
        const Value& with_what)
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<const ForwardRange> ));
    return std::replace_copy(hboost::begin(rng), hboost::end(rng), out_it,
        what, with_what);
}

    } // namespace range
    using range::replace_copy;
} // namespace hboost

#endif // include guard