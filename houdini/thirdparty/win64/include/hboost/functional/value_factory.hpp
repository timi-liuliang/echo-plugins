/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger
  
    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#ifndef HBOOST_FUNCTIONAL_VALUE_FACTORY_HPP_INCLUDED
#   ifndef HBOOST_PP_IS_ITERATING

#     include <hboost/preprocessor/iteration/iterate.hpp>
#     include <hboost/preprocessor/repetition/enum_params.hpp>
#     include <hboost/preprocessor/repetition/enum_binary_params.hpp>

#     include <new>
#     include <hboost/pointee.hpp>
#     include <hboost/get_pointer.hpp>
#     include <hboost/non_type.hpp>
#     include <hboost/type_traits/remove_cv.hpp>

#     ifndef HBOOST_FUNCTIONAL_VALUE_FACTORY_MAX_ARITY
#       define HBOOST_FUNCTIONAL_VALUE_FACTORY_MAX_ARITY 10
#     elif HBOOST_FUNCTIONAL_VALUE_FACTORY_MAX_ARITY < 3
#       undef  HBOOST_FUNCTIONAL_VALUE_FACTORY_MAX_ARITY
#       define HBOOST_FUNCTIONAL_VALUE_FACTORY_MAX_ARITY 3
#     endif

namespace hboost
{
    template< typename T >
    class value_factory;

    //----- ---- --- -- - -  -   -

    template< typename T >
    class value_factory
    {
      public:
        typedef T result_type;

        value_factory()
        { }

#     define HBOOST_PP_FILENAME_1 <hboost/functional/value_factory.hpp>
#     define HBOOST_PP_ITERATION_LIMITS (0,HBOOST_FUNCTIONAL_VALUE_FACTORY_MAX_ARITY)
#     include HBOOST_PP_ITERATE()
    };

    template< typename T > class value_factory<T&>;
    // forbidden, would create a dangling reference
}
#     define HBOOST_FUNCTIONAL_VALUE_FACTORY_HPP_INCLUDED
#   else // defined(HBOOST_PP_IS_ITERATING)

#     define N HBOOST_PP_ITERATION()
#     if N > 0
    template< HBOOST_PP_ENUM_PARAMS(N, typename T) >
#     endif
    inline result_type operator()(HBOOST_PP_ENUM_BINARY_PARAMS(N,T,& a)) const
    {
        return result_type(HBOOST_PP_ENUM_PARAMS(N,a));
    }
#     undef N

#   endif // defined(HBOOST_PP_IS_ITERATING)

#endif // include guard

