
//  Copyright (c) 2014 Agustin Berge
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.


#ifndef HBOOST_TT_IS_FINAL_HPP_INCLUDED
#define HBOOST_TT_IS_FINAL_HPP_INCLUDED

#include <hboost/type_traits/intrinsics.hpp>
#include <hboost/type_traits/integral_constant.hpp>
#ifdef HBOOST_IS_FINAL
#include <hboost/type_traits/remove_cv.hpp>
#endif

namespace hboost {

#ifdef HBOOST_IS_FINAL
template <class T> struct is_final : public integral_constant<bool, HBOOST_IS_FINAL(typename remove_cv<T>::type)> {};
#else
template <class T> struct is_final : public integral_constant<bool, false> {};
#endif

} // namespace hboost

#endif // HBOOST_TT_IS_FINAL_HPP_INCLUDED
