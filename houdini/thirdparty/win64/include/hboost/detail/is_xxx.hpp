// Copyright David Abrahams 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_DETAIL_IS_XXX_DWA20051011_HPP
# define HBOOST_DETAIL_IS_XXX_DWA20051011_HPP

# include <hboost/config.hpp>
# include <hboost/mpl/bool.hpp>
# include <hboost/preprocessor/enum_params.hpp>


#  define HBOOST_DETAIL_IS_XXX_DEF(name, qualified_name, nargs)  \
template <class T>                                              \
struct is_##name : mpl::false_                                  \
{                                                               \
};                                                              \
                                                                \
template < HBOOST_PP_ENUM_PARAMS_Z(1, nargs, class T) >          \
struct is_##name<                                               \
   qualified_name< HBOOST_PP_ENUM_PARAMS_Z(1, nargs, T) >        \
>                                                               \
   : mpl::true_                                                 \
{                                                               \
};


#endif // HBOOST_DETAIL_IS_XXX_DWA20051011_HPP
