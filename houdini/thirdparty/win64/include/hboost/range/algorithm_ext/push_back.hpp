// Boost.Range library
//
//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_EXT_PUSH_BACK_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_EXT_PUSH_BACK_HPP_INCLUDED

#include <hboost/range/config.hpp>
#include <hboost/range/concepts.hpp>
#include <hboost/range/difference_type.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/detail/implementation_help.hpp>
#include <hboost/assert.hpp>

namespace hboost
{
    namespace range
    {

template< class Container, class Range >
inline Container& push_back( Container& on, const Range& from )
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<Container> ));
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const Range> ));
    HBOOST_ASSERT_MSG(!range_detail::is_same_object(on, from),
        "cannot copy from a container to itself");
    on.insert( on.end(), hboost::begin(from), hboost::end(from) );
    return on;
}

    } // namespace range
    using range::push_back;
} // namespace hboost

#endif // include guard
