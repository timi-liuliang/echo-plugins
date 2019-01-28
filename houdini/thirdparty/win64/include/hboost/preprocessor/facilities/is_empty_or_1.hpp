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
# ifndef HBOOST_PREPROCESSOR_FACILITIES_IS_EMPTY_OR_1_HPP
# define HBOOST_PREPROCESSOR_FACILITIES_IS_EMPTY_OR_1_HPP
#
# include <hboost/preprocessor/control/iif.hpp>
# include <hboost/preprocessor/facilities/empty.hpp>
# include <hboost/preprocessor/facilities/identity.hpp>
# include <hboost/preprocessor/facilities/is_1.hpp>
# include <hboost/preprocessor/facilities/is_empty.hpp>
#
# /* HBOOST_PP_IS_EMPTY_OR_1 */
#
# define HBOOST_PP_IS_EMPTY_OR_1(x) \
    HBOOST_PP_IIF( \
        HBOOST_PP_IS_EMPTY(x HBOOST_PP_EMPTY()), \
        HBOOST_PP_IDENTITY(1), \
        HBOOST_PP_IS_1 \
    )(x) \
    /**/
#
# endif
