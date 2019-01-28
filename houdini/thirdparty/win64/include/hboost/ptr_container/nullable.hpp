//
// Boost.Pointer Container
//
//  Copyright Thorsten Ottosen 2003-2005. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/ptr_container/
//


#ifndef HBOOST_INDIRECT_CONTAINER_NULLABLE_HPP
#define HBOOST_INDIRECT_CONTAINER_NULLABLE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <hboost/type_traits/detail/yes_no_type.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/config.hpp>

namespace hboost
{
    
    template< class T >
    struct nullable
    {
        typedef T type;
    };   

    namespace ptr_container_detail
    {
        template< class T >
        type_traits::yes_type is_nullable( const nullable<T>* );

        type_traits::no_type is_nullable( ... );        
    }

    template< class T >
    struct is_nullable
    {
    private:
            HBOOST_STATIC_CONSTANT( T*, var );
    public:

#if HBOOST_WORKAROUND(HBOOST_MSVC, >= 1400)  
#pragma warning(push)  
#pragma warning(disable:6334)  
#endif  

            HBOOST_STATIC_CONSTANT(bool, value = sizeof( ptr_container_detail::is_nullable( var ) ) 
                                                == sizeof( type_traits::yes_type ) );
#if HBOOST_WORKAROUND(HBOOST_MSVC, >= 1400)  
#pragma warning(pop)  
#endif  
            
    };
    
    template< class T >
    struct remove_nullable
    {
        typedef HBOOST_DEDUCED_TYPENAME mpl::eval_if< is_nullable<T>,
                                                      T,
                                            mpl::identity<T> >::type
            type;
    };

}

#endif