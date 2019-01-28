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

#ifndef HBOOST_PTR_CONTAINER_SCOPED_DELETER_HPP
#define HBOOST_PTR_CONTAINER_SCOPED_DELETER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <iterator>
#include <cstddef>
#include <hboost/scoped_array.hpp>

namespace hboost
{

    namespace ptr_container_detail
    {
        template< class T, class CloneAllocator >
        class scoped_deleter
        {
            typedef std::size_t size_type;
            scoped_array<T*>  ptrs_;
            size_type         stored_; 
            bool              released_;
            
        public:
            scoped_deleter( T** a, size_type size ) 
                : ptrs_( a ), stored_( size ), released_( false )
            { 
                HBOOST_ASSERT( a );
            }
            
            scoped_deleter( size_type size ) 
                : ptrs_( new T*[size] ), stored_( 0 ), 
                 released_( false )
            {
                HBOOST_ASSERT( size > 0 );
            }


            
            scoped_deleter( size_type n, const T& x ) // strong
                : ptrs_( new T*[n] ), stored_(0),
                  released_( false )
            {
                for( size_type i = 0; i != n; i++ )
                    add( CloneAllocator::allocate_clone( &x ) );
                HBOOST_ASSERT( stored_ > 0 );
            }


            
            template< class InputIterator >
            scoped_deleter ( InputIterator first, InputIterator last  ) // strong
                : ptrs_( new T*[ std::distance(first,last) ] ),
                  stored_(0),
                  released_( false )
            {
                for( ; first != last; ++first )
                    add( CloneAllocator::allocate_clone_from_iterator( first ) );
                HBOOST_ASSERT( stored_ > 0 );
            }

            
            
            ~scoped_deleter()
            {
                if ( !released_ )
                {
                    for( size_type i = 0u; i != stored_; ++i )
                        CloneAllocator::deallocate_clone( ptrs_[i] ); 
                }
            }
            
            
            
            void add( T* t )
            {
                HBOOST_ASSERT( ptrs_.get() != 0 );
                ptrs_[stored_] = t;
                ++stored_;
            }
            
            
            
            void release()
            {
                released_ = true;
            }
            
            
            
            T** begin()
            {
                HBOOST_ASSERT( ptrs_.get() != 0 );
                return &ptrs_[0];
            }
            
            
            
            T** end()
            {
                HBOOST_ASSERT( ptrs_.get() != 0 );
                return &ptrs_[stored_];
            }
            
        }; // class 'scoped_deleter'
    }
}

#endif
