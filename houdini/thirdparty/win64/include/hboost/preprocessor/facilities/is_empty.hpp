# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2003.
#  *     (C) Copyright Edward Diener 2014.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef HBOOST_PREPROCESSOR_FACILITIES_IS_EMPTY_HPP
# define HBOOST_PREPROCESSOR_FACILITIES_IS_EMPTY_HPP
#
# include <hboost/preprocessor/config/config.hpp>
#
# if HBOOST_PP_VARIADICS
#
# include <hboost/preprocessor/facilities/is_empty_variadic.hpp>
#
# else
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MSVC() && ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
# include <hboost/preprocessor/tuple/elem.hpp>
# include <hboost/preprocessor/facilities/identity.hpp>
# else
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/detail/split.hpp>
# endif
#
# /* HBOOST_PP_IS_EMPTY */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MSVC() && ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
#    define HBOOST_PP_IS_EMPTY(x) HBOOST_PP_IS_EMPTY_I(x HBOOST_PP_IS_EMPTY_HELPER)
#    define HBOOST_PP_IS_EMPTY_I(contents) HBOOST_PP_TUPLE_ELEM(2, 1, (HBOOST_PP_IS_EMPTY_DEF_ ## contents()))
#    define HBOOST_PP_IS_EMPTY_DEF_HBOOST_PP_IS_EMPTY_HELPER 1, HBOOST_PP_IDENTITY(1)
#    define HBOOST_PP_IS_EMPTY_HELPER() , 0
# else
#    if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MSVC()
#        define HBOOST_PP_IS_EMPTY(x) HBOOST_PP_IS_EMPTY_I(HBOOST_PP_IS_EMPTY_HELPER x ())
#        define HBOOST_PP_IS_EMPTY_I(test) HBOOST_PP_IS_EMPTY_II(HBOOST_PP_SPLIT(0, HBOOST_PP_CAT(HBOOST_PP_IS_EMPTY_DEF_, test)))
#        define HBOOST_PP_IS_EMPTY_II(id) id
#    else
#        define HBOOST_PP_IS_EMPTY(x) HBOOST_PP_IS_EMPTY_I((HBOOST_PP_IS_EMPTY_HELPER x ()))
#        define HBOOST_PP_IS_EMPTY_I(par) HBOOST_PP_IS_EMPTY_II ## par
#        define HBOOST_PP_IS_EMPTY_II(test) HBOOST_PP_SPLIT(0, HBOOST_PP_CAT(HBOOST_PP_IS_EMPTY_DEF_, test))
#    endif
#    define HBOOST_PP_IS_EMPTY_HELPER() 1
#    define HBOOST_PP_IS_EMPTY_DEF_1 1, HBOOST_PP_NIL
#    define HBOOST_PP_IS_EMPTY_DEF_HBOOST_PP_IS_EMPTY_HELPER 0, HBOOST_PP_NIL
# endif
#
# endif /* HBOOST_PP_VARIADICS */
#
# endif /* HBOOST_PREPROCESSOR_FACILITIES_IS_EMPTY_HPP */
