// Boost.Assign library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/assign/
//

#ifndef HBOOST_ASSIGN_STD_SLIST_HPP
#define HBOOST_ASSIGN_STD_SLIST_HPP

#include <hboost/config.hpp>
#ifdef HBOOST_HAS_SLIST

#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/assign/list_inserter.hpp>
#ifdef HBOOST_SLIST_HEADER
# include HBOOST_SLIST_HEADER
#else
# include <slist>
#endif

namespace hboost
{
namespace assign
{

    template< class V, class A, class V2 >
    inline list_inserter< assign_detail::call_push_back< HBOOST_STD_EXTENSION_NAMESPACE::slist<V,A> >, V >
    operator+=( HBOOST_STD_EXTENSION_NAMESPACE::slist<V,A>& c, V2 v )
    {
        return push_back( c )( v );
    }
    
}
}

#endif // HBOOST_HAS_SLIST

#endif 
