// Copyright David Abrahams 2003.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_VALUE_IS_XXX_DWA2003224_HPP
# define HBOOST_VALUE_IS_XXX_DWA2003224_HPP

# include <hboost/config.hpp>
# include <hboost/mpl/bool.hpp>
# include <hboost/preprocessor/enum_params.hpp>


#  include <hboost/type_traits/remove_reference.hpp>
#  include <hboost/type_traits/remove_cv.hpp>
#  include <hboost/python/detail/is_xxx.hpp>

#  define HBOOST_PYTHON_VALUE_IS_XXX_DEF(name, qualified_name, nargs)    \
template <class X_>                                                     \
struct value_is_##name                                                  \
{                                                                       \
    HBOOST_PYTHON_IS_XXX_DEF(name,qualified_name,nargs)                  \
    HBOOST_STATIC_CONSTANT(bool, value = is_##name<                      \
                               typename remove_cv<                      \
                                  typename remove_reference<X_>::type   \
                               >::type                                  \
                           >::value);                                   \
    typedef mpl::bool_<value> type;                                    \
                                                                        \
};                                                              


#endif // HBOOST_VALUE_IS_XXX_DWA2003224_HPP
