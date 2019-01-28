// Boost.Range library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef HBOOST_RANGE_RBEGIN_HPP
#define HBOOST_RANGE_RBEGIN_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/range/end.hpp>
#include <hboost/range/reverse_iterator.hpp>

namespace hboost
{

#ifdef HBOOST_NO_FUNCTION_TEMPLATE_ORDERING

template< class C >
inline HBOOST_DEDUCED_TYPENAME range_reverse_iterator<C>::type
rbegin( C& c )
{
    return HBOOST_DEDUCED_TYPENAME range_reverse_iterator<C>::type( hboost::end( c ) );
}

#else

template< class C >
inline HBOOST_DEDUCED_TYPENAME range_reverse_iterator<C>::type
rbegin( C& c )
{
    typedef HBOOST_DEDUCED_TYPENAME range_reverse_iterator<C>::type
        iter_type;
    return iter_type( hboost::end( c ) );
}

template< class C >
inline HBOOST_DEDUCED_TYPENAME range_reverse_iterator<const C>::type
rbegin( const C& c )
{
    typedef HBOOST_DEDUCED_TYPENAME range_reverse_iterator<const C>::type
        iter_type;
    return iter_type( hboost::end( c ) );
}

#endif // HBOOST_NO_FUNCTION_TEMPLATE_ORDERING

template< class T >
inline HBOOST_DEDUCED_TYPENAME range_reverse_iterator<const T>::type
const_rbegin( const T& r )
{
    return hboost::rbegin( r );
}

} // namespace 'boost'

#endif

