// Boost.Range library
//
//  Copyright Thorsten Ottosen 2003-2006. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef HBOOST_RANGE_DISTANCE_HPP
#define HBOOST_RANGE_DISTANCE_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/difference_type.hpp>

namespace hboost 
{

    template< class T >
    inline HBOOST_DEDUCED_TYPENAME range_difference<T>::type 
    distance( const T& r )
    {
        return std::distance( hboost::begin( r ), hboost::end( r ) );
    }

} // namespace 'boost'

#endif
