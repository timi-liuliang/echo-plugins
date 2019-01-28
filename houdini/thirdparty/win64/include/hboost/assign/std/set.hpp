// Boost.Assign library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/assign/
//


#ifndef HBOOST_ASSIGN_STD_SET_HPP
#define HBOOST_ASSIGN_STD_SET_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/assign/list_inserter.hpp>
#include <hboost/config.hpp>
#include <set>

namespace hboost
{
namespace assign
{
    template< class K, class C, class A, class K2 >
    inline list_inserter< assign_detail::call_insert< std::set<K,C,A> >, K > 
    operator+=( std::set<K,C,A>& c, K2 k )
    {
        return insert( c )( k );
    }
    
    template< class K, class C, class A, class K2 >
    inline list_inserter< assign_detail::call_insert< std::multiset<K,C,A> >, K > 
    operator+=( std::multiset<K,C,A>& c, K2 k )
    {
        return insert( c )( k );
    }
    
}
}

#endif
