# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2014.                                    *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef HBOOST_PREPROCESSOR_VARIADIC_DETAIL_IS_SINGLE_RETURN_HPP
# define HBOOST_PREPROCESSOR_VARIADIC_DETAIL_IS_SINGLE_RETURN_HPP
#
# include <hboost/preprocessor/config/config.hpp>
#
# /* HBOOST_PP_VARIADIC_IS_SINGLE_RETURN */
#
# if HBOOST_PP_VARIADICS && HBOOST_PP_VARIADICS_MSVC
# include <hboost/preprocessor/control/iif.hpp>
# include <hboost/preprocessor/facilities/is_1.hpp>
# include <hboost/preprocessor/variadic/size.hpp>
# define HBOOST_PP_VARIADIC_IS_SINGLE_RETURN(sr,nsr,...)	\
	HBOOST_PP_IIF(HBOOST_PP_IS_1(HBOOST_PP_VARIADIC_SIZE(__VA_ARGS__)),sr,nsr) \
	/**/
# endif /* HBOOST_PP_VARIADICS && HBOOST_PP_VARIADICS_MSVC */
#
# endif /* HBOOST_PREPROCESSOR_VARIADIC_DETAIL_IS_SINGLE_RETURN_HPP */
