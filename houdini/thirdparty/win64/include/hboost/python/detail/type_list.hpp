// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_TYPE_LIST_DWA2002913_HPP
# define HBOOST_TYPE_LIST_DWA2002913_HPP

# include <hboost/config.hpp>
# include <hboost/python/detail/preprocessor.hpp>
# include <hboost/preprocessor/arithmetic/inc.hpp>

# if HBOOST_PYTHON_MAX_ARITY + 2 > HBOOST_PYTHON_MAX_BASES
#  define HBOOST_PYTHON_LIST_SIZE HBOOST_PP_INC(HBOOST_PP_INC(HBOOST_PYTHON_MAX_ARITY))
# else
#  define HBOOST_PYTHON_LIST_SIZE HBOOST_PYTHON_MAX_BASES
# endif

// Compute the MPL vector header to use for lists up to HBOOST_PYTHON_LIST_SIZE in length
# if HBOOST_PYTHON_LIST_SIZE > 48
#  error Arities above 48 not supported by Boost.Python due to MPL internal limit
# elif HBOOST_PYTHON_LIST_SIZE > 38
#  include <hboost/mpl/vector/vector50.hpp>
# elif HBOOST_PYTHON_LIST_SIZE > 28
#  include <hboost/mpl/vector/vector40.hpp>
# elif HBOOST_PYTHON_LIST_SIZE > 18
#  include <hboost/mpl/vector/vector30.hpp>
# elif HBOOST_PYTHON_LIST_SIZE > 8
#  include <hboost/mpl/vector/vector20.hpp>
# else
#  include <hboost/mpl/vector/vector10.hpp>
# endif

#  include <hboost/python/detail/type_list_impl.hpp>

#endif // HBOOST_TYPE_LIST_DWA2002913_HPP
