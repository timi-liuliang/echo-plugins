
#if !defined(HBOOST_PP_IS_ITERATING)

///// header body

// NO INCLUDE GUARDS, THE HEADER IS INTENDED FOR MULTIPLE INCLUSION!

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

#if !defined(HBOOST_MPL_PREPROCESSING_MODE)
#   include <hboost/mpl/numeric_cast.hpp>
#   include <hboost/mpl/apply_wrap.hpp>
#   include <hboost/mpl/if.hpp>
#   include <hboost/mpl/tag.hpp>
#   include <hboost/mpl/aux_/numeric_cast_utils.hpp>
#   include <hboost/mpl/aux_/na.hpp>
#   include <hboost/mpl/aux_/na_spec.hpp>
#   include <hboost/mpl/aux_/lambda_support.hpp>
#   include <hboost/mpl/aux_/msvc_eti_base.hpp>
#   include <hboost/mpl/aux_/value_wknd.hpp>
#   include <hboost/mpl/aux_/config/eti.hpp>
#   include <hboost/mpl/aux_/nttp_decl.hpp>
#endif

#include <hboost/mpl/aux_/config/static_constant.hpp>

#if defined(HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    || defined(HBOOST_MPL_PREPROCESSING_MODE)

#   include <hboost/mpl/limits/arity.hpp>
#   include <hboost/mpl/aux_/preprocessor/partial_spec_params.hpp>
#   include <hboost/mpl/aux_/preprocessor/def_params_tail.hpp>
#   include <hboost/mpl/aux_/preprocessor/repeat.hpp>
#   include <hboost/mpl/aux_/preprocessor/ext_params.hpp>
#   include <hboost/mpl/aux_/preprocessor/params.hpp>
#   include <hboost/mpl/aux_/preprocessor/enum.hpp>
#   include <hboost/mpl/aux_/preprocessor/add.hpp>
#   include <hboost/mpl/aux_/preprocessor/sub.hpp>
#   include <hboost/mpl/aux_/config/ctps.hpp>
#   include <hboost/mpl/aux_/config/eti.hpp>
#   include <hboost/mpl/aux_/config/msvc.hpp>
#   include <hboost/mpl/aux_/config/workaround.hpp>

#   include <hboost/preprocessor/dec.hpp>
#   include <hboost/preprocessor/inc.hpp>
#   include <hboost/preprocessor/iterate.hpp>
#   include <hboost/preprocessor/cat.hpp>


#if !defined(AUX778076_OP_ARITY)
#   define AUX778076_OP_ARITY HBOOST_MPL_LIMIT_METAFUNCTION_ARITY
#endif

#if !defined(AUX778076_OP_IMPL_NAME)
#   define AUX778076_OP_IMPL_NAME HBOOST_PP_CAT(AUX778076_OP_PREFIX,_impl)
#endif

#if !defined(AUX778076_OP_TAG_NAME)
#   define AUX778076_OP_TAG_NAME HBOOST_PP_CAT(AUX778076_OP_PREFIX,_tag)
#endif

namespace hboost { namespace mpl {

template< 
      typename Tag1
    , typename Tag2
#if HBOOST_WORKAROUND(HBOOST_MSVC, <= 1300)
    , HBOOST_MPL_AUX_NTTP_DECL(int, tag1_) = HBOOST_MPL_AUX_MSVC_VALUE_WKND(Tag1)::value 
    , HBOOST_MPL_AUX_NTTP_DECL(int, tag2_) = HBOOST_MPL_AUX_MSVC_VALUE_WKND(Tag2)::value 
    >
struct AUX778076_OP_IMPL_NAME
    : if_c<
          ( tag1_ > tag2_ )
#else
    >
struct AUX778076_OP_IMPL_NAME
    : if_c<
          ( HBOOST_MPL_AUX_NESTED_VALUE_WKND(int, Tag1)
              > HBOOST_MPL_AUX_NESTED_VALUE_WKND(int, Tag2)
            )
#endif
        , aux::cast2nd_impl< AUX778076_OP_IMPL_NAME<Tag1,Tag1>,Tag1,Tag2 >
        , aux::cast1st_impl< AUX778076_OP_IMPL_NAME<Tag2,Tag2>,Tag1,Tag2 >
        >::type
{
};

/// for Digital Mars C++/compilers with no CTPS/TTP support
template<> struct AUX778076_OP_IMPL_NAME<na,na>
{
    template< typename U1, typename U2 > struct apply 
    {
        typedef apply type;
        HBOOST_STATIC_CONSTANT(int, value = 0);
    };
};

#if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
template< typename Tag > struct AUX778076_OP_IMPL_NAME<na,Tag>
{
    template< typename U1, typename U2 > struct apply 
    {
        typedef apply type;
        HBOOST_STATIC_CONSTANT(int, value = 0);
    };
};

template< typename Tag > struct AUX778076_OP_IMPL_NAME<Tag,na>
{
    template< typename U1, typename U2 > struct apply 
    {
        typedef apply type;
        HBOOST_STATIC_CONSTANT(int, value = 0);
    };
};
#else
template<> struct AUX778076_OP_IMPL_NAME<na,integral_c_tag>
{
    template< typename U1, typename U2 > struct apply 
    {
        typedef apply type;
        HBOOST_STATIC_CONSTANT(int, value = 0);
    };
};

template<> struct AUX778076_OP_IMPL_NAME<integral_c_tag,na>
{
    template< typename U1, typename U2 > struct apply 
    {
        typedef apply type;
        HBOOST_STATIC_CONSTANT(int, value = 0);
    };
};
#endif


#if defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
    && HBOOST_WORKAROUND(HBOOST_MSVC, >= 1300)
template< typename T > struct AUX778076_OP_TAG_NAME
    : tag<T,na>
{
};
#else
template< typename T > struct AUX778076_OP_TAG_NAME
{
    typedef typename T::tag type;
};
#endif


#if AUX778076_OP_ARITY != 2

#   if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

#   define AUX778076_OP_RIGHT_OPERAND(unused, i, N) , HBOOST_PP_CAT(N, HBOOST_MPL_PP_ADD(i, 2))>
#   define AUX778076_OP_N_CALLS(i, N) \
    HBOOST_MPL_PP_REPEAT( HBOOST_PP_DEC(i), HBOOST_MPL_PP_REPEAT_IDENTITY_FUNC, AUX778076_OP_NAME< ) \
    N1 HBOOST_MPL_PP_REPEAT( HBOOST_MPL_PP_SUB(i, 1), AUX778076_OP_RIGHT_OPERAND, N ) \
/**/

template<
      typename HBOOST_MPL_AUX_NA_PARAM(N1)
    , typename HBOOST_MPL_AUX_NA_PARAM(N2)
    HBOOST_MPL_PP_DEF_PARAMS_TAIL(2, typename N, na)
    >
struct AUX778076_OP_NAME
    : AUX778076_OP_N_CALLS(AUX778076_OP_ARITY, N)
{
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(
          AUX778076_OP_ARITY
        , AUX778076_OP_NAME
        , ( HBOOST_MPL_PP_PARAMS(AUX778076_OP_ARITY, N) )
        )
};

#define HBOOST_PP_ITERATION_PARAMS_1 \
    (3,( HBOOST_PP_DEC(AUX778076_OP_ARITY), 2, <hboost/mpl/aux_/numeric_op.hpp> ))
#include HBOOST_PP_ITERATE()

#   undef AUX778076_OP_N_CALLS
#   undef AUX778076_OP_RIGHT_OPERAND

#   else // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

/// forward declaration
template< 
      typename HBOOST_MPL_AUX_NA_PARAM(N1)
    , typename HBOOST_MPL_AUX_NA_PARAM(N2)
    >
struct HBOOST_PP_CAT(AUX778076_OP_NAME,2);

template<
      typename HBOOST_MPL_AUX_NA_PARAM(N1)
    , typename HBOOST_MPL_AUX_NA_PARAM(N2)
    HBOOST_MPL_PP_DEF_PARAMS_TAIL(2, typename N, na)
    >
struct AUX778076_OP_NAME
#if HBOOST_WORKAROUND(HBOOST_MSVC, == 1300)
    : aux::msvc_eti_base< typename if_<
#else
    : if_<
#endif
          is_na<N3>
        , HBOOST_PP_CAT(AUX778076_OP_NAME,2)<N1,N2>
        , AUX778076_OP_NAME<
              HBOOST_PP_CAT(AUX778076_OP_NAME,2)<N1,N2>
            , HBOOST_MPL_PP_EXT_PARAMS(3, HBOOST_PP_INC(AUX778076_OP_ARITY), N)
            >
        >::type
#if HBOOST_WORKAROUND(HBOOST_MSVC, == 1300)
    >
#endif
{
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(
          AUX778076_OP_ARITY
        , AUX778076_OP_NAME
        , ( HBOOST_MPL_PP_PARAMS(AUX778076_OP_ARITY, N) )
        )
};

template< 
      typename N1
    , typename N2
    >
struct HBOOST_PP_CAT(AUX778076_OP_NAME,2)

#endif

#else // AUX778076_OP_ARITY == 2

template< 
      typename HBOOST_MPL_AUX_NA_PARAM(N1)
    , typename HBOOST_MPL_AUX_NA_PARAM(N2)
    >
struct AUX778076_OP_NAME

#endif

#if !defined(HBOOST_MPL_CFG_MSVC_ETI_BUG)
    : AUX778076_OP_IMPL_NAME<
          typename AUX778076_OP_TAG_NAME<N1>::type
        , typename AUX778076_OP_TAG_NAME<N2>::type
        >::template apply<N1,N2>::type
#else
    : aux::msvc_eti_base< typename apply_wrap2<
          AUX778076_OP_IMPL_NAME<
              typename AUX778076_OP_TAG_NAME<N1>::type
            , typename AUX778076_OP_TAG_NAME<N2>::type
            >
        , N1
        , N2
        >::type >::type
#endif
{
#if AUX778076_OP_ARITY != 2

#   if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
    HBOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(
          AUX778076_OP_ARITY
        , AUX778076_OP_NAME
        , ( HBOOST_MPL_PP_PARTIAL_SPEC_PARAMS(2, N, na) )
        )
#   else
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(2, HBOOST_PP_CAT(AUX778076_OP_NAME,2), (N1, N2))
#   endif

#else
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(2, AUX778076_OP_NAME, (N1, N2))
#endif
};

HBOOST_MPL_AUX_NA_SPEC2(2, AUX778076_OP_ARITY, AUX778076_OP_NAME)

}}

#endif // HBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS

///// iteration, depth == 1

// For gcc 4.4 compatability, we must include the
// HBOOST_PP_ITERATION_DEPTH test inside an #else clause.
#else // HBOOST_PP_IS_ITERATING
#if HBOOST_PP_ITERATION_DEPTH() == 1

#   define i_ HBOOST_PP_FRAME_ITERATION(1)

template<
      HBOOST_MPL_PP_PARAMS(i_, typename N)
    >
struct AUX778076_OP_NAME<HBOOST_MPL_PP_PARTIAL_SPEC_PARAMS(i_, N, na)>
#if i_ != 2
    : AUX778076_OP_N_CALLS(i_, N)
{
    HBOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(
          AUX778076_OP_ARITY
        , AUX778076_OP_NAME
        , ( HBOOST_MPL_PP_PARTIAL_SPEC_PARAMS(i_, N, na) )
        )
};
#endif

#   undef i_

#endif // HBOOST_PP_ITERATION_DEPTH()
#endif // HBOOST_PP_IS_ITERATING
