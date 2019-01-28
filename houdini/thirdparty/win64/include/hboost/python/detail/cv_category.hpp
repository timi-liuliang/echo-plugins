// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_CV_CATEGORY_DWA200222_HPP
# define HBOOST_CV_CATEGORY_DWA200222_HPP
# include <hboost/type_traits/cv_traits.hpp>

namespace hboost { namespace python { namespace detail { 

template <bool is_const_, bool is_volatile_>
struct cv_tag
{
    HBOOST_STATIC_CONSTANT(bool, is_const = is_const_);
    HBOOST_STATIC_CONSTANT(bool, is_volatile = is_volatile_);
};

typedef cv_tag<false,false> cv_unqualified;
typedef cv_tag<true,false> const_;
typedef cv_tag<false,true> volatile_;
typedef cv_tag<true,true> const_volatile_;

template <class T>
struct cv_category
{
//    HBOOST_STATIC_CONSTANT(bool, c = is_const<T>::value);
//    HBOOST_STATIC_CONSTANT(bool, v = is_volatile<T>::value);
    typedef cv_tag<
        ::hboost::is_const<T>::value
      , ::hboost::is_volatile<T>::value
    > type;
};

}}} // namespace hboost::python::detail

#endif // HBOOST_CV_CATEGORY_DWA200222_HPP
