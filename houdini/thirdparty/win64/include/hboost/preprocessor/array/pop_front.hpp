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
# ifndef HBOOST_PREPROCESSOR_ARRAY_POP_FRONT_HPP
# define HBOOST_PREPROCESSOR_ARRAY_POP_FRONT_HPP
#
# include <hboost/preprocessor/arithmetic/dec.hpp>
# include <hboost/preprocessor/arithmetic/inc.hpp>
# include <hboost/preprocessor/array/elem.hpp>
# include <hboost/preprocessor/array/size.hpp>
# include <hboost/preprocessor/repetition/enum.hpp>
# include <hboost/preprocessor/repetition/deduce_z.hpp>
#
# /* HBOOST_PP_ARRAY_POP_FRONT */
#
# define HBOOST_PP_ARRAY_POP_FRONT(array) HBOOST_PP_ARRAY_POP_FRONT_Z(HBOOST_PP_DEDUCE_Z(), array)
#
# /* HBOOST_PP_ARRAY_POP_FRONT_Z */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_EDG()
#    define HBOOST_PP_ARRAY_POP_FRONT_Z(z, array) HBOOST_PP_ARRAY_POP_FRONT_I(z, HBOOST_PP_ARRAY_SIZE(array), array)
# else
#    define HBOOST_PP_ARRAY_POP_FRONT_Z(z, array) HBOOST_PP_ARRAY_POP_FRONT_Z_D(z, array)
#    define HBOOST_PP_ARRAY_POP_FRONT_Z_D(z, array) HBOOST_PP_ARRAY_POP_FRONT_I(z, HBOOST_PP_ARRAY_SIZE(array), array)
# endif
#
# define HBOOST_PP_ARRAY_POP_FRONT_I(z, size, array) (HBOOST_PP_DEC(size), (HBOOST_PP_ENUM_ ## z(HBOOST_PP_DEC(size), HBOOST_PP_ARRAY_POP_FRONT_M, array)))
# define HBOOST_PP_ARRAY_POP_FRONT_M(z, n, data) HBOOST_PP_ARRAY_ELEM(HBOOST_PP_INC(n), data)
#
# endif
