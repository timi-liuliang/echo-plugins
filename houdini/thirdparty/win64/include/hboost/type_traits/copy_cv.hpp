#ifndef HBOOST_TYPE_TRAITS_COPY_CV_HPP_INCLUDED
#define HBOOST_TYPE_TRAITS_COPY_CV_HPP_INCLUDED

//
//  Copyright 2015 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//

#include <hboost/type_traits/is_const.hpp>
#include <hboost/type_traits/is_volatile.hpp>
#include <hboost/type_traits/add_const.hpp>
#include <hboost/type_traits/add_volatile.hpp>
#include <hboost/type_traits/conditional.hpp>

namespace hboost
{

template<class T, class U> struct copy_cv
{
private:

    typedef typename hboost::conditional<hboost::is_const<U>::value, typename hboost::add_const<T>::type, T>::type CT;

public:

    typedef typename hboost::conditional<hboost::is_volatile<U>::value, typename hboost::add_volatile<CT>::type, CT>::type type;
};

} // namespace hboost

#endif // #ifndef HBOOST_TYPE_TRAITS_COPY_CV_HPP_INCLUDED
