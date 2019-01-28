# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2003.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef HBOOST_PREPROCESSOR_FACILITIES_IS_1_HPP
# define HBOOST_PREPROCESSOR_FACILITIES_IS_1_HPP
#
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/facilities/is_empty.hpp>
#
# /* HBOOST_PP_IS_1 */
#
# define HBOOST_PP_IS_1(x) HBOOST_PP_IS_EMPTY(HBOOST_PP_CAT(HBOOST_PP_IS_1_HELPER_, x))
# define HBOOST_PP_IS_1_HELPER_1
#
# endif
