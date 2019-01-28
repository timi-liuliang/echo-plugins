#ifndef HBOOST_SMART_PTR_DETAIL_ATOMIC_COUNT_NT_HPP_INCLUDED
#define HBOOST_SMART_PTR_DETAIL_ATOMIC_COUNT_NT_HPP_INCLUDED

//
//  hboost/detail/atomic_count_nt.hpp
//
//  Trivial atomic_count for the single-threaded case
//
//  http://gcc.gnu.org/onlinedocs/porting/Thread-safety.html
//
//  Copyright 2013 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//

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
        return ++value_;
    }

    long operator--()
    {
        return --value_;
    }

    operator long() const
    {
        return value_;
    }

private:

    atomic_count(atomic_count const &);
    atomic_count & operator=(atomic_count const &);

    long value_;
};

} // namespace detail

} // namespace hboost

#endif // #ifndef HBOOST_SMART_PTR_DETAIL_ATOMIC_COUNT_NT_HPP_INCLUDED
