// Boost.Function library

//  Copyright Douglas Gregor 2001-2003. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org/libs/function

// William Kempf, Jesse Jones and Karl Nelson were all very helpful in the
// design of this library.

#include <functional> // unary_function, binary_function

#include <hboost/preprocessor/iterate.hpp>
#include <hboost/detail/workaround.hpp>

#ifndef HBOOST_FUNCTION_MAX_ARGS
#  define HBOOST_FUNCTION_MAX_ARGS 10
#endif // HBOOST_FUNCTION_MAX_ARGS

// Include the prologue here so that the use of file-level iteration
// in anything that may be included by function_template.hpp doesn't break
#include <hboost/function/detail/prologue.hpp>

// Older Visual Age C++ version do not handle the file iteration well
#if HBOOST_WORKAROUND(__IBMCPP__, >= 500) && HBOOST_WORKAROUND(__IBMCPP__, < 800)
#  if HBOOST_FUNCTION_MAX_ARGS >= 0
#    include <hboost/function/function0.hpp>
#  endif
#  if HBOOST_FUNCTION_MAX_ARGS >= 1
#    include <hboost/function/function1.hpp>
#  endif
#  if HBOOST_FUNCTION_MAX_ARGS >= 2
#    include <hboost/function/function2.hpp>
#  endif
#  if HBOOST_FUNCTION_MAX_ARGS >= 3
#    include <hboost/function/function3.hpp>
#  endif
#  if HBOOST_FUNCTION_MAX_ARGS >= 4
#    include <hboost/function/function4.hpp>
#  endif
#  if HBOOST_FUNCTION_MAX_ARGS >= 5
#    include <hboost/function/function5.hpp>
#  endif
#  if HBOOST_FUNCTION_MAX_ARGS >= 6
#    include <hboost/function/function6.hpp>
#  endif
#  if HBOOST_FUNCTION_MAX_ARGS >= 7
#    include <hboost/function/function7.hpp>
#  endif
#  if HBOOST_FUNCTION_MAX_ARGS >= 8
#    include <hboost/function/function8.hpp>
#  endif
#  if HBOOST_FUNCTION_MAX_ARGS >= 9
#    include <hboost/function/function9.hpp>
#  endif
#  if HBOOST_FUNCTION_MAX_ARGS >= 10
#    include <hboost/function/function10.hpp>
#  endif
#else
// What is the '3' for?
#  define HBOOST_PP_ITERATION_PARAMS_1 (3,(0,HBOOST_FUNCTION_MAX_ARGS,<hboost/function/detail/function_iterate.hpp>))
#  include HBOOST_PP_ITERATE()
#  undef HBOOST_PP_ITERATION_PARAMS_1
#endif
