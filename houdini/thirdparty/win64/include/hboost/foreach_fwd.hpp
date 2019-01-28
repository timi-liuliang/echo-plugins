///////////////////////////////////////////////////////////////////////////////
// foreach.hpp header file
//
// Copyright 2010 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/foreach for documentation
//
// Credits:
// Kazutoshi Satoda: for suggesting the need for a _fwd header for foreach's
//                      customization points.

#ifndef HBOOST_FOREACH_FWD_HPP
#define HBOOST_FOREACH_FWD_HPP

// This must be at global scope, hence the uglified name
enum hboost_foreach_argument_dependent_lookup_hack
{
    hboost_foreach_argument_dependent_lookup_hack_value
};

namespace hboost
{

namespace foreach
{
    ///////////////////////////////////////////////////////////////////////////////
    // hboost::foreach::tag
    //
    typedef hboost_foreach_argument_dependent_lookup_hack tag;

    ///////////////////////////////////////////////////////////////////////////////
    // hboost::foreach::is_lightweight_proxy
    //   Specialize this for user-defined collection types if they are inexpensive to copy.
    //   This tells HBOOST_FOREACH it can avoid the rvalue/lvalue detection stuff.
    template<typename T>
    struct is_lightweight_proxy;

    ///////////////////////////////////////////////////////////////////////////////
    // hboost::foreach::is_noncopyable
    //   Specialize this for user-defined collection types if they cannot be copied.
    //   This also tells HBOOST_FOREACH to avoid the rvalue/lvalue detection stuff.
    template<typename T>
    struct is_noncopyable;

} // namespace foreach

} // namespace hboost

#endif