// Boost.Assign library
//
//  Copyright Thorsten Ottosen 2006. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/assign/
//

#ifndef HBOOST_ASSIGN_PTR_CONTAINER_PTR_MAP_INSERTER_HPP
#define HBOOST_ASSIGN_PTR_CONTAINER_PTR_MAP_INSERTER_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/assign/list_inserter.hpp>
#include <hboost/type_traits/remove_reference.hpp>
#include <hboost/type_traits/remove_pointer.hpp>

namespace hboost
{

namespace assign
{
    template< class PtrMap, class Obj > 
    class ptr_map_inserter
    {
        typedef HBOOST_DEDUCED_TYPENAME
                remove_pointer< HBOOST_DEDUCED_TYPENAME 
                       remove_reference<Obj>::type >::type
           obj_type;
        typedef HBOOST_DEDUCED_TYPENAME PtrMap::key_type
           key_type;
        
    public:
        
        ptr_map_inserter( PtrMap& m ) : m_( m )
        {}
        
        template< class Key >
        ptr_map_inserter& operator()( const Key& t )
        {
            key_type k(t);
            m_.insert( k, new obj_type );
            return *this;
        }

#ifndef HBOOST_ASSIGN_MAX_PARAMS // use user's value
#define HBOOST_ASSIGN_MAX_PARAMS 6        
#endif
#define HBOOST_ASSIGN_MAX_PARAMETERS (HBOOST_ASSIGN_MAX_PARAMS - 1)
#define HBOOST_ASSIGN_PARAMS1(n) HBOOST_PP_ENUM_PARAMS(n, class T)
#define HBOOST_ASSIGN_PARAMS2(n) HBOOST_PP_ENUM_BINARY_PARAMS(n, T, const& t)
#define HBOOST_ASSIGN_PARAMS3(n) HBOOST_PP_ENUM_PARAMS(n, t)
        
#define HBOOST_PP_LOCAL_LIMITS (1, HBOOST_ASSIGN_MAX_PARAMETERS)
#define HBOOST_PP_LOCAL_MACRO(n) \
    template< class T, HBOOST_ASSIGN_PARAMS1(n) > \
    ptr_map_inserter& operator()( const T& t, HBOOST_ASSIGN_PARAMS2(n) ) \
    { \
        key_type k(t); \
        m_.insert( k, new obj_type( HBOOST_ASSIGN_PARAMS3(n) ) ); \
        return *this; \
    } \
    /**/
        
#include HBOOST_PP_LOCAL_ITERATE()

    private:

        ptr_map_inserter& operator=( const ptr_map_inserter& );
        PtrMap& m_;
    };
    
    template< class PtrMap >
    inline ptr_map_inserter< PtrMap, typename PtrMap::mapped_reference >
    ptr_map_insert( PtrMap& m )
    {
        return ptr_map_inserter< PtrMap, typename PtrMap::mapped_reference >( m );
    }

#ifndef HBOOST_NO_FUNCTION_TEMPLATE_ORDERING

    template< class T, class PtrMap >
    inline ptr_map_inserter< PtrMap, T >
    ptr_map_insert( PtrMap& m )
    {
        return ptr_map_inserter< PtrMap, T >( m );
    }

#endif
     
} // namespace 'assign'
} // namespace 'boost'

#undef HBOOST_ASSIGN_PARAMS1
#undef HBOOST_ASSIGN_PARAMS2
#undef HBOOST_ASSIGN_PARAMS3
#undef HBOOST_ASSIGN_MAX_PARAMETERS

#endif
