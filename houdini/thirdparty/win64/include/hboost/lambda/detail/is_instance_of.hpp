// Boost Lambda Library - is_instance_of.hpp ---------------------

// Copyright (C) 2001 Jaakko Jarvi (jaakko.jarvi@cs.utu.fi)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org

// ---------------------------------------------------------------

#ifndef HBOOST_LAMBDA_IS_INSTANCE_OF
#define HBOOST_LAMBDA_IS_INSTANCE_OF

#include "hboost/config.hpp" // for HBOOST_STATIC_CONSTANT
#include "hboost/type_traits/conversion_traits.hpp" // for is_convertible
#include "hboost/preprocessor/enum_shifted_params.hpp"
#include "hboost/preprocessor/repeat_2nd.hpp"

// is_instance_of --------------------------------
// 
// is_instance_of_n<A, B>::value is true, if type A is 
// an instantiation of a template B, or A derives from an instantiation 
// of template B
//
// n is the number of template arguments for B
// 
// Example:
// is_instance_of_2<std::istream, basic_stream>::value == true

// The original implementation was somewhat different, with different versions
// for different compilers. However, there was still a problem
// with gcc.3.0.2 and 3.0.3 compilers, which didn't think regard
// is_instance_of_N<...>::value was a constant.
// John Maddock suggested the way around this problem by building 
// is_instance_of templates using hboost::is_convertible.
// Now we only have one version of is_instance_of templates, which delagate
// all the nasty compiler tricks to is_convertible. 

#define HBOOST_LAMBDA_CLASS(z, N,A) HBOOST_PP_COMMA_IF(N) class
#define HBOOST_LAMBDA_CLASS_ARG(z, N,A) HBOOST_PP_COMMA_IF(N) class A##N 
#define HBOOST_LAMBDA_ARG(z, N,A) HBOOST_PP_COMMA_IF(N) A##N 

#define HBOOST_LAMBDA_CLASS_LIST(n, NAME) HBOOST_PP_REPEAT(n, HBOOST_LAMBDA_CLASS, NAME)

#define HBOOST_LAMBDA_CLASS_ARG_LIST(n, NAME) HBOOST_PP_REPEAT(n, HBOOST_LAMBDA_CLASS_ARG, NAME)

#define HBOOST_LAMBDA_ARG_LIST(n, NAME) HBOOST_PP_REPEAT(n, HBOOST_LAMBDA_ARG, NAME)

namespace hboost {
namespace lambda {

#define HBOOST_LAMBDA_IS_INSTANCE_OF_TEMPLATE(INDEX)                         \
                                                                            \
namespace detail {                                                          \
                                                                            \
template <template<HBOOST_LAMBDA_CLASS_LIST(INDEX,T)> class F>               \
struct HBOOST_PP_CAT(conversion_tester_,INDEX) {                             \
  template<HBOOST_LAMBDA_CLASS_ARG_LIST(INDEX,A)>                            \
  HBOOST_PP_CAT(conversion_tester_,INDEX)                                    \
    (const F<HBOOST_LAMBDA_ARG_LIST(INDEX,A)>&);                             \
};                                                                          \
                                                                            \
} /* end detail */                                                          \
                                                                            \
template <class From, template <HBOOST_LAMBDA_CLASS_LIST(INDEX,T)> class To> \
struct HBOOST_PP_CAT(is_instance_of_,INDEX)                                  \
{                                                                           \
 private:                                                                   \
   typedef ::hboost::is_convertible<                                         \
     From,                                                                  \
     HBOOST_PP_CAT(detail::conversion_tester_,INDEX)<To>                     \
   > helper_type;                                                           \
                                                                            \
public:                                                                     \
  HBOOST_STATIC_CONSTANT(bool, value = helper_type::value);                  \
};


#define HBOOST_LAMBDA_HELPER(z, N, A) HBOOST_LAMBDA_IS_INSTANCE_OF_TEMPLATE( HBOOST_PP_INC(N) )

// Generate the traits for 1-4 argument templates

HBOOST_PP_REPEAT_2ND(4,HBOOST_LAMBDA_HELPER,FOO)

#undef HBOOST_LAMBDA_HELPER
#undef HBOOST_LAMBDA_IS_INSTANCE_OF_TEMPLATE
#undef HBOOST_LAMBDA_CLASS
#undef HBOOST_LAMBDA_ARG
#undef HBOOST_LAMBDA_CLASS_ARG
#undef HBOOST_LAMBDA_CLASS_LIST
#undef HBOOST_LAMBDA_ARG_LIST
#undef HBOOST_LAMBDA_CLASS_ARG_LIST

} // lambda
} // boost

#endif





