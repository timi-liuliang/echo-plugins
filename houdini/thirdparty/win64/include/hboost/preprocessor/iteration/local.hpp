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
# ifndef HBOOST_PREPROCESSOR_ITERATION_LOCAL_HPP
# define HBOOST_PREPROCESSOR_ITERATION_LOCAL_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/slot/slot.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
#
# /* HBOOST_PP_LOCAL_ITERATE */
#
# define HBOOST_PP_LOCAL_ITERATE() <hboost/preprocessor/iteration/detail/local.hpp>
#
# define HBOOST_PP_LOCAL_C(n) (HBOOST_PP_LOCAL_S) <= n && (HBOOST_PP_LOCAL_F) >= n
# define HBOOST_PP_LOCAL_R(n) (HBOOST_PP_LOCAL_F) <= n && (HBOOST_PP_LOCAL_S) >= n
#
# endif
