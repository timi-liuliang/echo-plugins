
// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

// NO INCLUDE GUARDS, THE HEADER IS INTENDED FOR MULTIPLE INCLUSION!

#if !defined(HBOOST_MPL_PREPROCESSING_MODE)
#   include <hboost/mpl/bool.hpp>
#   include <hboost/mpl/aux_/nested_type_wknd.hpp>
#   include <hboost/mpl/aux_/na_spec.hpp>
#   include <hboost/mpl/aux_/lambda_support.hpp>
#endif

#include <hboost/mpl/limits/arity.hpp>
#include <hboost/mpl/aux_/preprocessor/params.hpp>
#include <hboost/mpl/aux_/preprocessor/ext_params.hpp>
#include <hboost/mpl/aux_/preprocessor/def_params_tail.hpp>
#include <hboost/mpl/aux_/preprocessor/enum.hpp>
#include <hboost/mpl/aux_/preprocessor/sub.hpp>
#include <hboost/mpl/aux_/config/ctps.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

#include <hboost/preprocessor/dec.hpp>
#include <hboost/preprocessor/inc.hpp>
#include <hboost/preprocessor/cat.hpp>

namespace hboost { namespace mpl {

#   define AUX778076_PARAMS(param, sub) \
    HBOOST_MPL_PP_PARAMS( \
          HBOOST_MPL_PP_SUB(HBOOST_MPL_LIMIT_METAFUNCTION_ARITY, sub) \
        , param \
        ) \
    /**/

#   define AUX778076_SHIFTED_PARAMS(param, sub) \
    HBOOST_MPL_PP_EXT_PARAMS( \
          2, HBOOST_MPL_PP_SUB(HBOOST_PP_INC(HBOOST_MPL_LIMIT_METAFUNCTION_ARITY), sub) \
        , param \
        ) \
    /**/

#   define AUX778076_SPEC_PARAMS(param) \
    HBOOST_MPL_PP_ENUM( \
          HBOOST_PP_DEC(HBOOST_MPL_LIMIT_METAFUNCTION_ARITY) \
        , param \
        ) \
    /**/

namespace aux {

#if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template< bool C_, AUX778076_PARAMS(typename T, 1) >
struct HBOOST_PP_CAT(AUX778076_OP_NAME,impl)
    : HBOOST_PP_CAT(AUX778076_OP_VALUE1,_)
{
};

template< AUX778076_PARAMS(typename T, 1) >
struct HBOOST_PP_CAT(AUX778076_OP_NAME,impl)< AUX778076_OP_VALUE2,AUX778076_PARAMS(T, 1) >
    : HBOOST_PP_CAT(AUX778076_OP_NAME,impl)<
          HBOOST_MPL_AUX_NESTED_TYPE_WKND(T1)::value
        , AUX778076_SHIFTED_PARAMS(T, 1)
        , HBOOST_PP_CAT(AUX778076_OP_VALUE2,_)
        >
{
};

template<>
struct HBOOST_PP_CAT(AUX778076_OP_NAME,impl)<
          AUX778076_OP_VALUE2
        , AUX778076_SPEC_PARAMS(HBOOST_PP_CAT(AUX778076_OP_VALUE2,_))
        >
    : HBOOST_PP_CAT(AUX778076_OP_VALUE2,_)
{
};

#else

template< bool C_ > struct HBOOST_PP_CAT(AUX778076_OP_NAME,impl)
{
    template< AUX778076_PARAMS(typename T, 1) > struct result_
        : HBOOST_PP_CAT(AUX778076_OP_VALUE1,_)
    {
    };
};

template<> struct HBOOST_PP_CAT(AUX778076_OP_NAME,impl)<AUX778076_OP_VALUE2>
{
    template< AUX778076_PARAMS(typename T, 1) > struct result_
        : HBOOST_PP_CAT(AUX778076_OP_NAME,impl)< 
              HBOOST_MPL_AUX_NESTED_TYPE_WKND(T1)::value
            >::template result_< AUX778076_SHIFTED_PARAMS(T,1),HBOOST_PP_CAT(AUX778076_OP_VALUE2,_) >
    {
    };

#if HBOOST_WORKAROUND(HBOOST_MSVC, == 1300)
    template<> struct result_<AUX778076_SPEC_PARAMS(HBOOST_PP_CAT(AUX778076_OP_VALUE2,_))>
        : HBOOST_PP_CAT(AUX778076_OP_VALUE2,_)
    {
    };
};
#else
};

template<>
struct HBOOST_PP_CAT(AUX778076_OP_NAME,impl)<AUX778076_OP_VALUE2>
    ::result_< AUX778076_SPEC_PARAMS(HBOOST_PP_CAT(AUX778076_OP_VALUE2,_)) >
        : HBOOST_PP_CAT(AUX778076_OP_VALUE2,_)
{
};
#endif // HBOOST_MSVC == 1300

#endif // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace aux

template<
      typename HBOOST_MPL_AUX_NA_PARAM(T1)
    , typename HBOOST_MPL_AUX_NA_PARAM(T2)
    HBOOST_MPL_PP_DEF_PARAMS_TAIL(2, typename T, HBOOST_PP_CAT(AUX778076_OP_VALUE2,_))
    >
struct AUX778076_OP_NAME
#if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
    : aux::HBOOST_PP_CAT(AUX778076_OP_NAME,impl)<
          HBOOST_MPL_AUX_NESTED_TYPE_WKND(T1)::value
        , AUX778076_SHIFTED_PARAMS(T,0)
        >
#else
    : aux::HBOOST_PP_CAT(AUX778076_OP_NAME,impl)< 
          HBOOST_MPL_AUX_NESTED_TYPE_WKND(T1)::value
        >::template result_< AUX778076_SHIFTED_PARAMS(T,0) >
#endif
{
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(
          HBOOST_MPL_LIMIT_METAFUNCTION_ARITY
        , AUX778076_OP_NAME
        , (AUX778076_PARAMS(T, 0))
        )
};

HBOOST_MPL_AUX_NA_SPEC2(
      2
    , HBOOST_MPL_LIMIT_METAFUNCTION_ARITY
    , AUX778076_OP_NAME
    )

}}

#undef AUX778076_SPEC_PARAMS
#undef AUX778076_SHIFTED_PARAMS
#undef AUX778076_PARAMS
#undef AUX778076_OP_NAME
#undef AUX778076_OP_VALUE1
#undef AUX778076_OP_VALUE2
