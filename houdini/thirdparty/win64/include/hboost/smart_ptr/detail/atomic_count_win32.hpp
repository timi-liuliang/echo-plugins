#ifndef HBOOST_SMART_PTR_DETAIL_ATOMIC_COUNT_WIN32_HPP_INCLUDED
#define HBOOST_SMART_PTR_DETAIL_ATOMIC_COUNT_WIN32_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  hboost/detail/atomic_count_win32.hpp
//
//  Copyright (c) 2001-2005 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <hboost/smart_ptr/detail/sp_interlocked.hpp>

namespace hboost
{

namespace detail
{

class atomic_count
{
public:

    explicit atomic_count( long v ): value_( v )
    {
    }

    long operator++()
    {
        return HBOOST_SP_INTERLOCKED_INCREMENT( &value_ );
    }

    long operator--()
    {
        return HBOOST_SP_INTERLOCKED_DECREMENT( &value_ );
    }

    operator long() const
    {
        return static_cast<long const volatile &>( value_ );
    }

private:

    atomic_count( atomic_count const & );
    atomic_count & operator=( atomic_count const & );

    long value_;
};

} // namespace detail

} // namespace hboost

#endif // #ifndef HBOOST_SMART_PTR_DETAIL_ATOMIC_COUNT_WIN32_HPP_INCLUDED
