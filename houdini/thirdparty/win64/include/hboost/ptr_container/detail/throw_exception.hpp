//
// Boost.Pointer Container
//
//  Copyright Thorsten Ottosen 2006. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/ptr_container/
//

#ifndef HBOOST_PTR_CONTAINER_DETAIL_THROW_EXCEPTION
#define HBOOST_PTR_CONTAINER_DETAIL_THROW_EXCEPTION

#include <hboost/assert.hpp>
#include <hboost/config.hpp>

#ifdef HBOOST_NO_EXCEPTIONS
#define HBOOST_PTR_CONTAINER_NO_EXCEPTIONS
#endif

#ifdef HBOOST_PTR_CONTAINER_NO_EXCEPTIONS

#define HBOOST_PTR_CONTAINER_THROW_EXCEPTION( If, Ex, Msg ) HBOOST_ASSERT( !(If) && Msg ) 

#else

#define HBOOST_PTR_CONTAINER_THROW_EXCEPTION( If, Ex, Msg ) if( (If) ) throw Ex ( Msg )

#endif // HBOOST_PTR_CONTAINER_NO_EXCEPTIONS


#endif
