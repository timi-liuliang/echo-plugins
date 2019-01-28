//
// Boost.Pointer Container
//
//  Copyright Thorsten Ottosen 2003-2007. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/ptr_container/
//

#ifndef HBOOST_PTR_CONTAINER_INDIRECT_FUN_HPP
#define HBOOST_PTR_CONTAINER_INDIRECT_FUN_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
    #pragma once
#endif

#include <hboost/config.hpp>

#ifdef HBOOST_NO_SFINAE
#else
#include <hboost/utility/result_of.hpp>
#include <hboost/pointee.hpp>
#endif // HBOOST_NO_SFINAE

#include <hboost/assert.hpp>
#include <hboost/static_assert.hpp>
#include <hboost/type_traits/is_void.hpp>
#include <functional>


namespace hboost
{

    namespace ptr_container_detail
    {
        template <typename Type, typename Dummy>
        struct make_lazy
        {
            typedef typename Type::type type;
        };
    }
    
    template
    < 
              class Fun
#ifdef HBOOST_NO_SFINAE
            , class Result = bool
#endif        
    >
    class indirect_fun
    {
        Fun fun;
    public:
        indirect_fun() : fun(Fun())
        { }
        
        indirect_fun( Fun f ) : fun(f)
        { }
    
        template< class T >
#ifdef HBOOST_NO_SFINAE
        Result    
#else            
        typename hboost::result_of< const Fun( typename pointee<T>::type& ) >::type 
#endif            
        operator()( const T& r ) const
        { 
            return fun( *r );
        }
    
        template< class T, class U >
#ifdef HBOOST_NO_SFINAE
        Result    
#else                        
        typename hboost::result_of< const Fun( typename pointee<T>::type&,
                                              typename pointee<U>::type& ) >::type
#endif            
        operator()( const T& r, const U& r2 ) const
        { 
            return fun( *r, *r2 ); 
        }
    };

    template< class Fun >
    inline indirect_fun<Fun> make_indirect_fun( Fun f )
    {
        return indirect_fun<Fun>( f );
    }


    template
    < 
        class Fun, 
        class Arg1, 
        class Arg2 = Arg1 
#ifdef HBOOST_NO_SFINAE
      , class Result = bool   
#endif           
    >
    class void_ptr_indirect_fun
    {
        Fun fun;
                
    public:
        
        void_ptr_indirect_fun() : fun(Fun())
        { }

        void_ptr_indirect_fun( Fun f ) : fun(f)
        { }
        
        template< class Void >
#ifdef HBOOST_NO_SFINAE
        Result    
#else            
        typename ptr_container_detail::make_lazy<
            hboost::result_of<const Fun(const Arg1&)>, Void>::type
#endif            
        operator()( const Void* r ) const
        { 
            HBOOST_STATIC_ASSERT(hboost::is_void<Void>::value);
            HBOOST_ASSERT( r != 0 );
            return fun( * static_cast<const Arg1*>( r ) );
        }

        template< class Void >
#ifdef HBOOST_NO_SFINAE
        Result    
#else                    
        typename ptr_container_detail::make_lazy<
            hboost::result_of<const Fun(const Arg1&, const Arg2&)>, Void>::type
#endif            
        operator()( const Void* l, const Void* r ) const
        { 
            HBOOST_STATIC_ASSERT(hboost::is_void<Void>::value);
            HBOOST_ASSERT( l != 0 && r != 0 );
            return fun( * static_cast<const Arg1*>( l ), * static_cast<const Arg2*>( r ) );
        }
    };

    template< class Arg, class Fun >
    inline void_ptr_indirect_fun<Fun,Arg> make_void_ptr_indirect_fun( Fun f )
    {
        return void_ptr_indirect_fun<Fun,Arg>( f );
    }
     
} // namespace 'boost'

#endif
