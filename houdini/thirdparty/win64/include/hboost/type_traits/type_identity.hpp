#ifndef HBOOST_TYPE_TRAITS_TYPE_IDENTITY_HPP_INCLUDED
#define HBOOST_TYPE_TRAITS_TYPE_IDENTITY_HPP_INCLUDED

//
//  Copyright 2015 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//

namespace hboost
{

template<class T> struct type_identity
{
    typedef T type;
};

} // namespace hboost

#endif // #ifndef HBOOST_TYPE_TRAITS_TYPE_IDENTITY_HPP_INCLUDED