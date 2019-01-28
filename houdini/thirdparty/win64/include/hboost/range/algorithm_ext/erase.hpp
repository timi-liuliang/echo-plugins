// Boost.Range library
//
//  Copyright Neil Groves 2009. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//
#ifndef HBOOST_RANGE_ALGORITHM_EXT_ERASE_HPP_INCLUDED
#define HBOOST_RANGE_ALGORITHM_EXT_ERASE_HPP_INCLUDED

#include <hboost/range/config.hpp>
#include <hboost/range/concepts.hpp>
#include <hboost/range/difference_type.hpp>
#include <hboost/range/iterator_range_core.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/assert.hpp>

namespace hboost
{
    namespace range
    {

template< class Container >
inline Container& erase( Container& on,
      iterator_range<HBOOST_DEDUCED_TYPENAME Container::iterator> to_erase )
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<Container> ));
    on.erase( hboost::begin(to_erase), hboost::end(to_erase) );
    return on;
}

template< class Container, class T >
inline Container& remove_erase( Container& on, const T& val )
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<Container> ));
    on.erase(
        std::remove(hboost::begin(on), hboost::end(on), val),
        hboost::end(on));
    return on;
}

template< class Container, class Pred >
inline Container& remove_erase_if( Container& on, Pred pred )
{
    HBOOST_RANGE_CONCEPT_ASSERT(( ForwardRangeConcept<Container> ));
    on.erase(
        std::remove_if(hboost::begin(on), hboost::end(on), pred),
        hboost::end(on));
    return on;
}

    } // namespace range
    using range::erase;
    using range::remove_erase;
    using range::remove_erase_if;
} // namespace hboost

#endif // include guard
