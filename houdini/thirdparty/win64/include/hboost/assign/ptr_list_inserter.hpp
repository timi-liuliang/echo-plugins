// Boost.Assign library
//
//  Copyright Thorsten Ottosen 2003-2005. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/assign/
//

#ifndef HBOOST_ASSIGN_PTR_LIST_INSERTER_HPP
#define HBOOST_ASSIGN_PTR_LIST_INSERTER_HPP

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
    template< class Function, class Obj > 
    class ptr_list_inserter
    {
        typedef HBOOST_DEDUCED_TYPENAME
                remove_pointer< HBOOST_DEDUCED_TYPENAME 
                       remove_reference<Obj>::type >::type
           obj_type;
    public:
        
        ptr_list_inserter( Function fun ) : insert_( fun )
        {}
        
        template< class Function2, class Obj2 >
        ptr_list_inserter( const ptr_list_inserter<Function2,Obj2>& r ) 
        : insert_( r.fun_private() ) 
        {}

        ptr_list_inserter( const ptr_list_inserter& r ) : insert_( r.insert_ )
        {}

        ptr_list_inserter& operator()()
        {
            insert_( new obj_type() );
            return *this;
        }
        
        template< class T >
        ptr_list_inserter& operator()( const T& t )
        {
            insert_( new obj_type(t) );
            return *this;
        }

#ifndef HBOOST_ASSIGN_MAX_PARAMS // use user's value
#define HBOOST_ASSIGN_MAX_PARAMS 5        
#endif
#define HBOOST_ASSIGN_MAX_PARAMETERS (HBOOST_ASSIGN_MAX_PARAMS - 1)
#define HBOOST_ASSIGN_PARAMS1(n) HBOOST_PP_ENUM_PARAMS(n, class T)
#define HBOOST_ASSIGN_PARAMS2(n) HBOOST_PP_ENUM_BINARY_PARAMS(n, T, const& t)
#define HBOOST_ASSIGN_PARAMS3(n) HBOOST_PP_ENUM_PARAMS(n, t)
        
#define HBOOST_PP_LOCAL_LIMITS (1, HBOOST_ASSIGN_MAX_PARAMETERS)
#define HBOOST_PP_LOCAL_MACRO(n) \
    template< class T, HBOOST_ASSIGN_PARAMS1(n) > \
    ptr_list_inserter& operator()( const T& t, HBOOST_ASSIGN_PARAMS2(n) ) \
    { \
        insert_( new obj_type(t, HBOOST_ASSIGN_PARAMS3(n) )); \
        return *this; \
    } \
    /**/
        
#include HBOOST_PP_LOCAL_ITERATE()

    private:
        
        ptr_list_inserter& operator=( const ptr_list_inserter& );
        Function insert_;
    };
    
    template< class Obj, class Function >
    inline ptr_list_inserter< Function, Obj >
    make_ptr_list_inserter( Function fun )
    {
        return ptr_list_inserter< Function, Obj >( fun );
    }
    
    template< class C >
    inline ptr_list_inserter< assign_detail::call_push_back<C>, 
                              HBOOST_DEDUCED_TYPENAME C::reference >
    ptr_push_back( C& c )
    {
        return make_ptr_list_inserter<HBOOST_DEDUCED_TYPENAME C::reference>
                   ( assign_detail::call_push_back<C>( c ) ); 
    }

#ifndef HBOOST_NO_FUNCTION_TEMPLATE_ORDERING

    template< class T, class C >
    inline ptr_list_inserter< assign_detail::call_push_back<C>, T >
    ptr_push_back( C& c )
    {
        return make_ptr_list_inserter<T>( 
                    assign_detail::call_push_back<C>( c ) );
    }

#endif
    
    template< class C >
    inline ptr_list_inserter< assign_detail::call_push_front<C>,
                              HBOOST_DEDUCED_TYPENAME C::reference >
    ptr_push_front( C& c )
    {
        return make_ptr_list_inserter<HBOOST_DEDUCED_TYPENAME C::reference>
                 ( assign_detail::call_push_front<C>( c ) );
    }

#ifndef HBOOST_NO_FUNCTION_TEMPLATE_ORDERING

    template< class T, class C >
    inline ptr_list_inserter< assign_detail::call_push_front<C>, T >
    ptr_push_front( C& c )
    {
        return make_ptr_list_inserter<T>( 
                    assign_detail::call_push_front<C>( c ) );
    }

#endif
    
    template< class C >
    inline ptr_list_inserter< assign_detail::call_insert<C>, 
                          HBOOST_DEDUCED_TYPENAME C::reference>
    ptr_insert( C& c )
    {
        return make_ptr_list_inserter<HBOOST_DEDUCED_TYPENAME C::reference>
                    ( assign_detail::call_insert<C>( c ) );
    }

#ifndef HBOOST_NO_FUNCTION_TEMPLATE_ORDERING

    template< class T, class C >
    inline ptr_list_inserter< assign_detail::call_insert<C>, T >
    ptr_insert( C& c )
    {
        return make_ptr_list_inserter<T>( assign_detail::call_insert<C>( c ) );
    }

#endif
    
    
} // namespace 'assign'
} // namespace 'boost'

#undef HBOOST_ASSIGN_PARAMS1
#undef HBOOST_ASSIGN_PARAMS2
#undef HBOOST_ASSIGN_PARAMS3
#undef HBOOST_ASSIGN_MAX_PARAMETERS

#endif
