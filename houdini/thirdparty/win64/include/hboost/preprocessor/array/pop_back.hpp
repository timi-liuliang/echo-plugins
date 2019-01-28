# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef HBOOST_PREPROCESSOR_ARRAY_POP_BACK_HPP
# define HBOOST_PREPROCESSOR_ARRAY_POP_BACK_HPP
#
# include <hboost/preprocessor/arithmetic/dec.hpp>
# include <hboost/preprocessor/array/elem.hpp>
# include <hboost/preprocessor/array/size.hpp>
# include <hboost/preprocessor/repetition/enum.hpp>
# include <hboost/preprocessor/repetition/deduce_z.hpp>
#
# /* HBOOST_PP_ARRAY_POP_BACK */
#
# define HBOOST_PP_ARRAY_POP_BACK(array) HBOOST_PP_ARRAY_POP_BACK_Z(HBOOST_PP_DEDUCE_Z(), array)
#
# /* HBOOST_PP_ARRAY_POP_BACK_Z */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_ARRAY_POP_BACK_Z(z, array) HBOOST_PP_ARRAY_POP_BACK_I(z, HBOOST_PP_ARRAY_SIZE(array), array)
# else
#    define HBOOST_PP_ARRAY_POP_BACK_Z(z, array) HBOOST_PP_ARRAY_POP_BACK_Z_D(z, array)
#    define HBOOST_PP_ARRAY_POP_BACK_Z_D(z, array) HBOOST_PP_ARRAY_POP_BACK_I(z, HBOOST_PP_ARRAY_SIZE(array), array)
# endif
#
# define HBOOST_PP_ARRAY_POP_BACK_I(z, size, array) (HBOOST_PP_DEC(size), (HBOOST_PP_ENUM_ ## z(HBOOST_PP_DEC(size), HBOOST_PP_ARRAY_POP_BACK_M, array)))
# define HBOOST_PP_ARRAY_POP_BACK_M(z, n, data) HBOOST_PP_ARRAY_ELEM(n, data)
#
# endif
