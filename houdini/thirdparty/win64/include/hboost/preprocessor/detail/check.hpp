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
# ifndef HBOOST_PREPROCESSOR_DETAIL_CHECK_HPP
# define HBOOST_PREPROCESSOR_DETAIL_CHECK_HPP
#
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/config/config.hpp>
#
# /* HBOOST_PP_CHECK */
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
#    define HBOOST_PP_CHECK(x, type) HBOOST_PP_CHECK_D(x, type)
# else
#    define HBOOST_PP_CHECK(x, type) HBOOST_PP_CHECK_OO((x, type))
#    define HBOOST_PP_CHECK_OO(par) HBOOST_PP_CHECK_D ## par
# endif
#
# if ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MSVC() && ~HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_DMC()
#    define HBOOST_PP_CHECK_D(x, type) HBOOST_PP_CHECK_1(HBOOST_PP_CAT(HBOOST_PP_CHECK_RESULT_, type x))
#    define HBOOST_PP_CHECK_1(chk) HBOOST_PP_CHECK_2(chk)
#    define HBOOST_PP_CHECK_2(res, _) res
# elif HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MSVC()
#    define HBOOST_PP_CHECK_D(x, type) HBOOST_PP_CHECK_1(type x)
#    define HBOOST_PP_CHECK_1(chk) HBOOST_PP_CHECK_2(chk)
#    define HBOOST_PP_CHECK_2(chk) HBOOST_PP_CHECK_3((HBOOST_PP_CHECK_RESULT_ ## chk))
#    define HBOOST_PP_CHECK_3(im) HBOOST_PP_CHECK_5(HBOOST_PP_CHECK_4 im)
#    define HBOOST_PP_CHECK_4(res, _) res
#    define HBOOST_PP_CHECK_5(res) res
# else /* DMC */
#    define HBOOST_PP_CHECK_D(x, type) HBOOST_PP_CHECK_OO((type x))
#    define HBOOST_PP_CHECK_OO(par) HBOOST_PP_CHECK_0 ## par
#    define HBOOST_PP_CHECK_0(chk) HBOOST_PP_CHECK_1(HBOOST_PP_CAT(HBOOST_PP_CHECK_RESULT_, chk))
#    define HBOOST_PP_CHECK_1(chk) HBOOST_PP_CHECK_2(chk)
#    define HBOOST_PP_CHECK_2(res, _) res
# endif
#
# define HBOOST_PP_CHECK_RESULT_1 1, HBOOST_PP_NIL
#
# endif
