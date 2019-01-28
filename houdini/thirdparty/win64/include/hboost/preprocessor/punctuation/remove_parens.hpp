# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2014.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
#ifndef HBOOST_PREPROCESSOR_REMOVE_PARENS_HPP
#define HBOOST_PREPROCESSOR_REMOVE_PARENS_HPP

#include <hboost/preprocessor/config/config.hpp>

#if HBOOST_PP_VARIADICS

#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/facilities/identity.hpp>
#include <hboost/preprocessor/punctuation/is_begin_parens.hpp>
#include <hboost/preprocessor/tuple/enum.hpp>

#define HBOOST_PP_REMOVE_PARENS(param) \
    HBOOST_PP_IIF \
      ( \
      HBOOST_PP_IS_BEGIN_PARENS(param), \
      HBOOST_PP_REMOVE_PARENS_DO, \
      HBOOST_PP_IDENTITY \
      ) \
    (param)() \
/**/

#define HBOOST_PP_REMOVE_PARENS_DO(param) \
  HBOOST_PP_IDENTITY(HBOOST_PP_TUPLE_ENUM(param)) \
/**/

#endif /* HBOOST_PP_VARIADICS */
#endif /* HBOOST_PREPROCESSOR_REMOVE_PARENS_HPP */
