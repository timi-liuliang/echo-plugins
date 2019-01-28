
// NO INCLUDE GUARDS, THE HEADER IS INTENDED FOR MULTIPLE INCLUSION

#if !defined(HBOOST_PP_IS_ITERATING)

///// header body

// Copyright Aleksey Gurtovoy 2000-2008
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#   include <hboost/mpl/aux_/config/ctps.hpp>
#   include <hboost/mpl/aux_/config/static_constant.hpp>
#   include <hboost/mpl/aux_/nttp_decl.hpp>

#   include <hboost/preprocessor/arithmetic/sub.hpp>
#   include <hboost/preprocessor/tuple/elem.hpp>
#   include <hboost/preprocessor/enum_params_with_a_default.hpp>
#   include <hboost/preprocessor/enum_params.hpp>
#   include <hboost/preprocessor/enum.hpp>
#   include <hboost/preprocessor/repeat.hpp>
#   include <hboost/preprocessor/comma_if.hpp>
#   include <hboost/preprocessor/iterate.hpp>

#if defined(HBOOST_MPL_PREPROCESSING_MODE)
#   undef LONG_MAX
#endif

namespace hboost { namespace mpl {

#if !defined(AUX778076_SEQUENCE_BASE_NAME)
#   define AUX778076_SEQUENCE_BASE_NAME AUX778076_SEQUENCE_NAME
#endif

#if !defined(AUX778076_SEQUENCE_INTEGRAL_WRAPPER)

#   define AUX778076_SEQUENCE_PARAM_NAME T
#   define AUX778076_SEQUENCE_TEMPLATE_PARAM typename T
#   define AUX778076_SEQUENCE_DEFAULT na

#   define AUX778076_SEQUENCE_NAME_N(n) \
    HBOOST_PP_CAT(AUX778076_SEQUENCE_BASE_NAME,n) \
    /**/

#   define AUX778076_SEQUENCE_PARAMS() \
    HBOOST_PP_ENUM_PARAMS( \
          AUX778076_SEQUENCE_LIMIT \
        , AUX778076_SEQUENCE_TEMPLATE_PARAM \
        ) \
    /**/

#   define AUX778076_SEQUENCE_ARGS() \
    HBOOST_PP_ENUM_PARAMS( \
          AUX778076_SEQUENCE_LIMIT \
        , T \
        ) \
    /**/

#   define AUX778076_SEQUENCE_DEFAULT_PARAMS() \
     HBOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT( \
          AUX778076_SEQUENCE_LIMIT \
        , AUX778076_SEQUENCE_TEMPLATE_PARAM \
        , AUX778076_SEQUENCE_DEFAULT \
        ) \
    /**/

#   define AUX778076_SEQUENCE_N_PARAMS(n) \
    HBOOST_PP_ENUM_PARAMS(n, AUX778076_SEQUENCE_TEMPLATE_PARAM) \
    /**/

#   define AUX778076_SEQUENCE_N_ARGS(n) \
    HBOOST_PP_ENUM_PARAMS(n, T) \
    /**/

#   define AUX778076_SEQUENCE_N_PARTIAL_SPEC_ARGS(n) \
    HBOOST_PP_ENUM_PARAMS(n, T) \
    HBOOST_PP_COMMA_IF(n) \
    HBOOST_PP_ENUM( \
          HBOOST_PP_SUB_D(1,AUX778076_SEQUENCE_LIMIT,n) \
        , HBOOST_PP_TUPLE_ELEM_3_2 \
        , AUX778076_SEQUENCE_DEFAULT \
        ) \
    /**/

#else // AUX778076_SEQUENCE_INTEGRAL_WRAPPER

#   define AUX778076_SEQUENCE_PARAM_NAME C
#   define AUX778076_SEQUENCE_TEMPLATE_PARAM HBOOST_MPL_AUX_NTTP_DECL(long, C)
#   define AUX778076_SEQUENCE_DEFAULT LONG_MAX

#   define AUX778076_SEQUENCE_PARAMS() \
    typename T, HBOOST_PP_ENUM_PARAMS( \
          AUX778076_SEQUENCE_LIMIT \
        , AUX778076_SEQUENCE_TEMPLATE_PARAM \
        ) \
    /**/

#   define AUX778076_SEQUENCE_ARGS() \
    T, HBOOST_PP_ENUM_PARAMS( \
          AUX778076_SEQUENCE_LIMIT \
        , C \
        ) \
    /**/

#   define AUX778076_SEQUENCE_DEFAULT_PARAMS() \
    typename T, \
    HBOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT( \
          AUX778076_SEQUENCE_LIMIT \
        , AUX778076_SEQUENCE_TEMPLATE_PARAM \
        , AUX778076_SEQUENCE_DEFAULT \
        ) \
    /**/

#   define AUX778076_SEQUENCE_N_PARAMS(n) \
    typename T HBOOST_PP_COMMA_IF(n) \
    HBOOST_PP_ENUM_PARAMS(n, AUX778076_SEQUENCE_TEMPLATE_PARAM) \
    /**/

#   if !defined(AUX778076_SEQUENCE_CONVERT_CN_TO)
#       define AUX778076_SEQUENCE_CONVERT_CN_TO(z,n,TARGET) HBOOST_PP_CAT(C,n)
#   endif

#   define AUX778076_SEQUENCE_N_ARGS(n) \
    T HBOOST_PP_COMMA_IF(n) \
    HBOOST_PP_ENUM(n,AUX778076_SEQUENCE_CONVERT_CN_TO,T) \
    /**/

#   define AUX778076_SEQUENCE_N_PARTIAL_SPEC_ARGS(n) \
    T, HBOOST_PP_ENUM_PARAMS(n, C) \
    HBOOST_PP_COMMA_IF(n) \
    HBOOST_PP_ENUM( \
          HBOOST_PP_SUB_D(1,AUX778076_SEQUENCE_LIMIT,n) \
        , HBOOST_PP_TUPLE_ELEM_3_2 \
        , AUX778076_SEQUENCE_DEFAULT \
        ) \
    /**/

#endif // AUX778076_SEQUENCE_INTEGRAL_WRAPPER


#if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
// forward declaration
template<
      AUX778076_SEQUENCE_DEFAULT_PARAMS()
    >
struct AUX778076_SEQUENCE_NAME;
#else
namespace aux {
template< HBOOST_MPL_AUX_NTTP_DECL(int, N) > 
struct HBOOST_PP_CAT(AUX778076_SEQUENCE_NAME,_chooser);
}
#endif

#define HBOOST_PP_ITERATION_PARAMS_1 \
    (3,(0, AUX778076_SEQUENCE_LIMIT, <hboost/mpl/aux_/sequence_wrapper.hpp>))
#include HBOOST_PP_ITERATE()

// real C++ version is already taken care of
#if defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

namespace aux {
// ???_count_args
#define AUX778076_COUNT_ARGS_PREFIX         AUX778076_SEQUENCE_NAME
#define AUX778076_COUNT_ARGS_DEFAULT        AUX778076_SEQUENCE_DEFAULT
#define AUX778076_COUNT_ARGS_PARAM_NAME     AUX778076_SEQUENCE_PARAM_NAME
#define AUX778076_COUNT_ARGS_TEMPLATE_PARAM AUX778076_SEQUENCE_TEMPLATE_PARAM
#define AUX778076_COUNT_ARGS_ARITY          AUX778076_SEQUENCE_LIMIT
#define AUX778076_COUNT_ARGS_USE_STANDARD_PP_PRIMITIVES
#include <hboost/mpl/aux_/count_args.hpp>

template<
      AUX778076_SEQUENCE_PARAMS()
    >
struct HBOOST_PP_CAT(AUX778076_SEQUENCE_NAME,_impl)
{
    typedef aux::HBOOST_PP_CAT(AUX778076_SEQUENCE_NAME,_count_args)<
          HBOOST_PP_ENUM_PARAMS(AUX778076_SEQUENCE_LIMIT, AUX778076_SEQUENCE_PARAM_NAME)
        > arg_num_;
    
    typedef typename aux::HBOOST_PP_CAT(AUX778076_SEQUENCE_NAME,_chooser)< arg_num_::value >
        ::template result_< AUX778076_SEQUENCE_ARGS() >::type type;
};

} // namespace aux

template<
      AUX778076_SEQUENCE_DEFAULT_PARAMS()
    >
struct AUX778076_SEQUENCE_NAME
    : aux::HBOOST_PP_CAT(AUX778076_SEQUENCE_NAME,_impl)<
          AUX778076_SEQUENCE_ARGS()
        >::type
{
    typedef typename aux::HBOOST_PP_CAT(AUX778076_SEQUENCE_NAME,_impl)<
          AUX778076_SEQUENCE_ARGS()
        >::type type;
};

#endif // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#   undef AUX778076_SEQUENCE_N_PARTIAL_SPEC_ARGS
#   undef AUX778076_SEQUENCE_N_ARGS
#   undef AUX778076_SEQUENCE_CONVERT_CN_TO
#   undef AUX778076_SEQUENCE_N_PARAMS
#   undef AUX778076_SEQUENCE_DEFAULT_PARAMS
#   undef AUX778076_SEQUENCE_ARGS
#   undef AUX778076_SEQUENCE_PARAMS
#   undef AUX778076_SEQUENCE_NAME_N
#   undef AUX778076_SEQUENCE_DEFAULT
#   undef AUX778076_SEQUENCE_TEMPLATE_PARAM
#   undef AUX778076_SEQUENCE_PARAM_NAME
#   undef AUX778076_SEQUENCE_LIMIT
#   undef AUX778076_SEQUENCE_BASE_NAME
#   undef AUX778076_SEQUENCE_NAME
#   undef AUX778076_SEQUENCE_INTEGRAL_WRAPPER

}}

///// iteration

#else
#define i_ HBOOST_PP_FRAME_ITERATION(1)

#   if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

#if i_ == AUX778076_SEQUENCE_LIMIT

/// primary template (not a specialization!)
template<
      AUX778076_SEQUENCE_N_PARAMS(i_)
    >
struct AUX778076_SEQUENCE_NAME
    : AUX778076_SEQUENCE_NAME_N(i_)< AUX778076_SEQUENCE_N_ARGS(i_) >
{
    typedef typename AUX778076_SEQUENCE_NAME_N(i_)< AUX778076_SEQUENCE_N_ARGS(i_) >::type type;
};

#else

template<
      AUX778076_SEQUENCE_N_PARAMS(i_)
    >
struct AUX778076_SEQUENCE_NAME< AUX778076_SEQUENCE_N_PARTIAL_SPEC_ARGS(i_) >
    : AUX778076_SEQUENCE_NAME_N(i_)< AUX778076_SEQUENCE_N_ARGS(i_) >
{
#if i_ > 0 || defined(AUX778076_SEQUENCE_INTEGRAL_WRAPPER)
    typedef typename AUX778076_SEQUENCE_NAME_N(i_)< AUX778076_SEQUENCE_N_ARGS(i_) >::type type;
#else
    typedef AUX778076_SEQUENCE_NAME_N(i_)< AUX778076_SEQUENCE_N_ARGS(i_) >::type type;
#endif
};

#endif // i_ == AUX778076_SEQUENCE_LIMIT

#   else

namespace aux {

template<>
struct HBOOST_PP_CAT(AUX778076_SEQUENCE_NAME,_chooser)<i_>
{
    template<
          AUX778076_SEQUENCE_PARAMS()
        >
    struct result_
    {
#if i_ > 0 || defined(AUX778076_SEQUENCE_INTEGRAL_WRAPPER)
        typedef typename AUX778076_SEQUENCE_NAME_N(i_)<
              AUX778076_SEQUENCE_N_ARGS(i_)
            >::type type;
#else
        typedef AUX778076_SEQUENCE_NAME_N(i_)<
              AUX778076_SEQUENCE_N_ARGS(i_)
            >::type type;
#endif
    };
};

} // namespace aux

#   endif // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#undef i_
#endif // HBOOST_PP_IS_ITERATING
