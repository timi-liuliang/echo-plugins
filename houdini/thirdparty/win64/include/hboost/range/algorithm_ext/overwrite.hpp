// Boost.Range library
//
//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_EXT_OVERWRITE_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_EXT_OVERWRITE_HPP_INCLUDED

#include <hboost/range/config.hpp>
#include <hboost/range/concepts.hpp>
#include <hboost/range/difference_type.hpp>
#include <hboost/range/iterator.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/assert.hpp>

namespace hboost
{
    namespace range
    {

template< class SinglePassRange1, class SinglePassRange2 >
inline void overwrite( const SinglePassRange1& from, SinglePassRange2& to )
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange1> ));
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<SinglePassRange2> ));

    HBOOST_DEDUCED_TYPENAME range_iterator<const SinglePassRange1>::type
        i = hboost::begin(from), e = hboost::end(from);

    HBOOST_DEDUCED_TYPENAME range_iterator<SinglePassRange2>::type
        out = hboost::begin(to);

#ifndef NDEBUG
    HBOOST_DEDUCED_TYPENAME range_iterator<SinglePassRange2>::type
        last_out = hboost::end(to);
#endif

    for( ; i != e; ++out, ++i )
    {
#ifndef NDEBUG
        HBOOST_ASSERT( out != last_out
            && "out of bounds in hboost::overwrite()" );
#endif
        *out = *i;
    }
}

template< class SinglePassRange1, class SinglePassRange2 >
inline void overwrite( const SinglePassRange1& from, const SinglePassRange2& to )
{
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange1> ));
    HBOOST_RANGE_CONCEPT_ASSERT(( SinglePassRangeConcept<const SinglePassRange2> ));

    HBOOST_DEDUCED_TYPENAME range_iterator<const SinglePassRange1>::type
        i = hboost::begin(from), e = hboost::end(from);

    HBOOST_DEDUCED_TYPENAME range_iterator<const SinglePassRange2>::type
        out = hboost::begin(to);

#ifndef NDEBUG
    HBOOST_DEDUCED_TYPENAME range_iterator<const SinglePassRange2>::type
        last_out = hboost::end(to);
#endif

    for( ; i != e; ++out, ++i )
    {
#ifndef NDEBUG
        HBOOST_ASSERT( out != last_out
            && "out of bounds in hboost::overwrite()" );
#endif
        *out = *i;
    }
}

    } // namespace range
    using range::overwrite;
} // namespace hboost

#endif // include guard
