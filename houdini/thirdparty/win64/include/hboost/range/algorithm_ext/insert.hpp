// Boost.Range library
//
//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_EXT_INSERT_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_EXT_INSERT_HPP_INCLUDED

#include <hboost/range/config.hpp>
#include <hboost/range/concepts.hpp>
#include <hboost/range/difference_type.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/assert.hpp>

namespace hboost
{
    namespace range
    {

template< class Container, class Range >
inline Container& insert( Container& on,
                          HBOOST_DEDUCED_TYPENAME Container::iterator before,
                          const Range& from )
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<Container> ));
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<Range> ));
    on.insert( before, hboost::begin(from), hboost::end(from) );
    return on;
}

template< class Container, class Range >
inline Container& insert( Container& on, const Range& from )
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<Container> ));
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<Range> ));
    on.insert(hboost::begin(from), hboost::end(from));
}

    } // namespace range
    using range::insert;
} // namespace hboost

#endif // include guard
