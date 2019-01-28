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
# ifndef HBOOST_PREPROCESSOR_ARRAY_DETAIL_GET_DATA_HPP
# define HBOOST_PREPROCESSOR_ARRAY_DETAIL_GET_DATA_HPP
#
# include <hboost/preprocessor/config/config.hpp>
# include <hboost/preprocessor/tuple/rem.hpp>
# include <hboost/preprocessor/control/if.hpp>
# include <hboost/preprocessor/control/iif.hpp>
# include <hboost/preprocessor/facilities/is_1.hpp>
#
# /* HBOOST_PP_ARRAY_DETAIL_GET_DATA */
#
# define HBOOST_PP_ARRAY_DETAIL_GET_DATA_NONE(size, data)

# if HBOOST_PP_VARIADICS && !(HBOOST_PP_VARIADICS_MSVC && _MSC_VER <= 1400)
# 	 if HBOOST_PP_VARIADICS_MSVC
# 		define HBOOST_PP_ARRAY_DETAIL_GET_DATA_ANY_VC_DEFAULT(size, data) HBOOST_PP_TUPLE_REM(size) data
# 		define HBOOST_PP_ARRAY_DETAIL_GET_DATA_ANY_VC_CAT(size, data) HBOOST_PP_TUPLE_REM_CAT(size) data
# 		define HBOOST_PP_ARRAY_DETAIL_GET_DATA_ANY(size, data) \
			HBOOST_PP_IIF \
				( \
				HBOOST_PP_IS_1(size), \
				HBOOST_PP_ARRAY_DETAIL_GET_DATA_ANY_VC_CAT, \
				HBOOST_PP_ARRAY_DETAIL_GET_DATA_ANY_VC_DEFAULT \
				) \
			(size,data) \
/**/
#    else
# 		define HBOOST_PP_ARRAY_DETAIL_GET_DATA_ANY(size, data) HBOOST_PP_TUPLE_REM(size) data
#    endif
# else
# 	 define HBOOST_PP_ARRAY_DETAIL_GET_DATA_ANY(size, data) HBOOST_PP_TUPLE_REM(size) data
# endif

# define HBOOST_PP_ARRAY_DETAIL_GET_DATA(size, data) \
	HBOOST_PP_IF \
		( \
		size, \
		HBOOST_PP_ARRAY_DETAIL_GET_DATA_ANY, \
		HBOOST_PP_ARRAY_DETAIL_GET_DATA_NONE \
		) \
	(size,data) \
/**/
#
# endif /* HBOOST_PREPROCESSOR_ARRAY_DETAIL_GET_DATA_HPP */
