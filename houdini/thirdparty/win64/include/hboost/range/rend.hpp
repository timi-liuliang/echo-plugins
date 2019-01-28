// Boost.Range library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef HBOOST_RANGE_REND_HPP
#define HBOOST_RANGE_REND_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/range/begin.hpp>
#include <hboost/range/reverse_iterator.hpp>

namespace hboost
{

#ifdef HBOOST_NO_FUNCTION_TEMPLATE_ORDERING

template< class C >
inline HBOOST_DEDUCED_TYPENAME range_reverse_iterator<C>::type
rend( C& c )
{
    return HBOOST_DEDUCED_TYPENAME range_reverse_iterator<C>::type( hboost::begin( c ) );
}

#else

template< class C >
inline HBOOST_DEDUCED_TYPENAME range_reverse_iterator<C>::type
rend( C& c )
{
    typedef HBOOST_DEDUCED_TYPENAME range_reverse_iterator<C>::type
               iter_type;
    return iter_type( hboost::begin( c ) );
}

template< class C >
inline HBOOST_DEDUCED_TYPENAME range_reverse_iterator<const C>::type
rend( const C& c )
{
    typedef HBOOST_DEDUCED_TYPENAME range_reverse_iterator<const C>::type
        iter_type;
    return iter_type( hboost::begin( c ) );
}

#endif

template< class T >
inline HBOOST_DEDUCED_TYPENAME range_reverse_iterator<const T>::type
const_rend( const T& r )
{
    return hboost::rend( r );
}

} // namespace 'boost'

#endif

