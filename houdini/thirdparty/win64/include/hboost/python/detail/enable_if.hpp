// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_ENABLE_IF_DWA2004722_HPP
# define HBOOST_ENABLE_IF_DWA2004722_HPP

# include <hboost/python/detail/sfinae.hpp>
# include <hboost/detail/workaround.hpp>

#if !defined(HBOOST_NO_SFINAE)
#  include <hboost/utility/enable_if.hpp>

namespace hboost { namespace python { namespace detail { 

template <class C, class T = int>
struct enable_if_arg
  : enable_if<C,T>
{};
             
template <class C, class T = int>
struct disable_if_arg
  : disable_if<C,T>
{};
             
template <class C, class T = void>
struct enable_if_ret
  : enable_if<C,T>
{};
             
template <class C, class T = void>
struct disable_if_ret
  : disable_if<C,T>
{};
             
}}} // namespace hboost::python::detail

# endif

#endif // HBOOST_ENABLE_IF_DWA2004722_HPP
