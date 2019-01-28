/*=============================================================================
    Copyright (c) 2010-2011 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HBOOST_FUSION_ADAPTED_DETAIL_STRUCT_DEFINE_STRUCT_HPP
#define HBOOST_FUSION_ADAPTED_DETAIL_STRUCT_DEFINE_STRUCT_HPP

#include <hboost/fusion/support/config.hpp>
#include <hboost/config.hpp>
#include <hboost/fusion/sequence/intrinsic/begin.hpp>
#include <hboost/fusion/iterator/deref.hpp>
#include <hboost/fusion/iterator/next.hpp>
#include <hboost/fusion/iterator/advance.hpp>
#include <hboost/fusion/adapted/struct/detail/adapt_base.hpp>
#include <hboost/fusion/adapted/struct/detail/namespace.hpp>
#include <hboost/preprocessor/if.hpp>
#include <hboost/preprocessor/expr_if.hpp>
#include <hboost/preprocessor/dec.hpp>
#include <hboost/preprocessor/logical/not.hpp>
#include <hboost/preprocessor/punctuation/comma_if.hpp>
#include <hboost/preprocessor/seq/seq.hpp>
#include <hboost/preprocessor/seq/for_each.hpp>
#include <hboost/preprocessor/seq/for_each_i.hpp>
#include <hboost/preprocessor/seq/enum.hpp>
#include <hboost/preprocessor/seq/size.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/tuple/eat.hpp>
#include <hboost/call_traits.hpp>
#include <hboost/type_traits/is_convertible.hpp>
#include <hboost/utility/enable_if.hpp>

#define HBOOST_FUSION_DEFINE_STRUCT_FILLER_0(X, Y)                               \
    ((X, Y)) HBOOST_FUSION_DEFINE_STRUCT_FILLER_1
#define HBOOST_FUSION_DEFINE_STRUCT_FILLER_1(X, Y)                               \
    ((X, Y)) HBOOST_FUSION_DEFINE_STRUCT_FILLER_0
#define HBOOST_FUSION_DEFINE_STRUCT_FILLER_0_END
#define HBOOST_FUSION_DEFINE_STRUCT_FILLER_1_END

#define HBOOST_FUSION_DEFINE_STRUCT_COPY_CTOR_FILLER_I(                          \
    R, ATTRIBUTE_TUPLE_SIZE, I, ATTRIBUTE)                                      \
                                                                                \
    HBOOST_PP_COMMA_IF(I)                                                        \
    HBOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPLE_SIZE,1,ATTRIBUTE)(                      \
        other_self.HBOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPLE_SIZE,1,ATTRIBUTE))

#define HBOOST_FUSION_DEFINE_STRUCT_ASSIGN_FILLER_I(                             \
    R, ATTRIBUTE_TUPLE_SIZE, I_, ATTRIBUTE)                                     \
                                                                                \
    HBOOST_PP_EXPR_IF(                                                           \
        I_,                                                                     \
        typedef typename                                                        \
            hboost::fusion::result_of::next<                                     \
                HBOOST_PP_CAT(I,HBOOST_PP_DEC(I_))                                \
            >::type                                                             \
        HBOOST_PP_CAT(I,I_);                                                     \
        HBOOST_PP_CAT(I,I_) HBOOST_PP_CAT(i,I_)=                                  \
                hboost::fusion::next(HBOOST_PP_CAT(i,HBOOST_PP_DEC(I_)));          \
    )                                                                           \
                                                                                \
    HBOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPLE_SIZE,1,ATTRIBUTE)=                      \
        hboost::fusion::deref(HBOOST_PP_CAT(i,I_));

#define HBOOST_FUSION_DEFINE_STRUCT_ASSIGN_OP(                                   \
    ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)                                       \
                                                                                \
    template<typename Seq>                                                      \
    HBOOST_FUSION_GPU_ENABLED                                                    \
    self_type&                                                                  \
    operator=(Seq const& seq)                                                   \
    {                                                                           \
        typedef typename                                                        \
            hboost::fusion::result_of::begin<Seq const>::type                    \
        I0;                                                                     \
        I0 i0=hboost::fusion::begin(seq);                                        \
                                                                                \
        HBOOST_PP_SEQ_FOR_EACH_I_R(                                              \
            1,                                                                  \
            HBOOST_FUSION_DEFINE_STRUCT_ASSIGN_FILLER_I,                         \
            ATTRIBUTE_TUPLE_SIZE,                                               \
            ATTRIBUTES_SEQ)                                                     \
                                                                                \
        return *this;                                                           \
    }

#define HBOOST_FUSION_DEFINE_STRUCT_ATTR_I(R, ATTRIBUTE_TUPLE_SIZE, ATTRIBUTE)   \
                                                                                \
    HBOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPLE_SIZE,0,ATTRIBUTE)                       \
        HBOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPLE_SIZE,1,ATTRIBUTE);

#define HBOOST_FUSION_DEFINE_STRUCT_SEQ_CTOR_FILLER_I(                           \
    R, ATTRIBUTE_TUPLE_SIZE, I, ATTRIBUTE)                                      \
                                                                                \
    HBOOST_PP_COMMA_IF(I)                                                        \
    HBOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPLE_SIZE,1,ATTRIBUTE)(                      \
        hboost::fusion::deref(hboost::fusion::advance_c<I>(hboost::fusion::begin(  \
            seq))))

#define HBOOST_FUSION_DEFINE_STRUCT_SEQ_CTOR_DISABLER(                           \
    ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)                                       \
                                                                                \
    , typename hboost::disable_if<                                               \
        hboost::is_convertible<                                                  \
            Seq const&                                                          \
          , HBOOST_PP_TUPLE_ELEM(                                                \
                ATTRIBUTE_TUPLE_SIZE,                                           \
                0,                                                              \
                HBOOST_PP_SEQ_HEAD(ATTRIBUTES_SEQ))                              \
        >                                                                       \
    >::type* =0

#define HBOOST_FUSION_DEFINE_STRUCT_SEQ_DEFAULT_CTOR_FILLER_I(                   \
    R, ATTRIBUTE_TUPLE_SIZE, I, ATTRIBUTE)                                      \
                                                                                \
    HBOOST_PP_COMMA_IF(I)                                                        \
    HBOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPLE_SIZE,1,ATTRIBUTE)()

#define HBOOST_FUSION_DEFINE_STRUCT_IMPL_IMPL(                                   \
    NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)                                 \
                                                                                \
    HBOOST_PP_SEQ_FOR_EACH_R(                                                    \
        1,                                                                      \
        HBOOST_FUSION_DEFINE_STRUCT_ATTR_I,                                      \
        ATTRIBUTE_TUPLE_SIZE,                                                   \
        ATTRIBUTES_SEQ)                                                         \
                                                                                \
    HBOOST_FUSION_GPU_ENABLED                                                    \
    NAME()                                                                      \
      : HBOOST_PP_SEQ_FOR_EACH_I_R(                                              \
            1,                                                                  \
            HBOOST_FUSION_DEFINE_STRUCT_SEQ_DEFAULT_CTOR_FILLER_I,               \
            ATTRIBUTE_TUPLE_SIZE,                                               \
            ATTRIBUTES_SEQ)                                                     \
    {}                                                                          \
                                                                                \
    HBOOST_FUSION_GPU_ENABLED                                                    \
    NAME(self_type const& other_self)                                           \
      : HBOOST_PP_SEQ_FOR_EACH_I_R(                                              \
            1,                                                                  \
            HBOOST_FUSION_DEFINE_STRUCT_COPY_CTOR_FILLER_I,                      \
            ATTRIBUTE_TUPLE_SIZE,                                               \
            ATTRIBUTES_SEQ)                                                     \
    {}                                                                          \
                                                                                \
    template<typename Seq>                                                      \
    HBOOST_FUSION_GPU_ENABLED                                                    \
    NAME(Seq const& seq                                                         \
        HBOOST_PP_IF(                                                            \
            HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(ATTRIBUTES_SEQ)),                    \
            HBOOST_PP_TUPLE_EAT(2),                                              \
            HBOOST_FUSION_DEFINE_STRUCT_SEQ_CTOR_DISABLER)(                      \
                ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)                           \
        )                                                                       \
      : HBOOST_PP_SEQ_FOR_EACH_I_R(                                              \
            1,                                                                  \
            HBOOST_FUSION_DEFINE_STRUCT_SEQ_CTOR_FILLER_I,                       \
            ATTRIBUTE_TUPLE_SIZE,                                               \
            ATTRIBUTES_SEQ)                                                     \
    {}                                                                          \
                                                                                \
    HBOOST_FUSION_DEFINE_STRUCT_ASSIGN_OP(ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)

#define HBOOST_FUSION_DEFINE_STRUCT_CTOR_1(                                      \
        NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)                             \
                                                                                \
        HBOOST_FUSION_GPU_ENABLED                                                \
        explicit                                                                \
        NAME(hboost::call_traits<                                                \
                HBOOST_PP_TUPLE_ELEM(                                            \
                    ATTRIBUTE_TUPLE_SIZE,0,HBOOST_PP_SEQ_HEAD(ATTRIBUTES_SEQ))   \
            >::param_type arg)                                                  \
          : HBOOST_PP_TUPLE_ELEM(                                                \
                ATTRIBUTE_TUPLE_SIZE,1,HBOOST_PP_SEQ_HEAD(ATTRIBUTES_SEQ))(arg)  \
        {}

#define HBOOST_FUSION_DEFINE_TPL_STRUCT_CTOR_1(                                  \
        TEMPLATE_PARAMS_SEQ, NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)        \
                                                                                \
        HBOOST_FUSION_GPU_ENABLED                                                \
        explicit                                                                \
        NAME(typename hboost::call_traits<                                       \
                typename hboost::fusion::detail::get_first_arg<                  \
                    HBOOST_PP_TUPLE_ELEM(                                        \
                        ATTRIBUTE_TUPLE_SIZE,                                   \
                        0,                                                      \
                        HBOOST_PP_SEQ_HEAD(ATTRIBUTES_SEQ))                      \
                  , HBOOST_PP_SEQ_HEAD(TEMPLATE_PARAMS_SEQ)                      \
                >::type                                                         \
            >::param_type arg)                                                  \
          : HBOOST_PP_TUPLE_ELEM(                                                \
                ATTRIBUTE_TUPLE_SIZE,1,HBOOST_PP_SEQ_HEAD(ATTRIBUTES_SEQ))(arg)  \
        {}

#define HBOOST_FUSION_DEFINE_STRUCT_CTOR_FILLER_I(                               \
    R, ATTRIBUTE_TUPLE_SIZE, I, ATTRIBUTE)                                      \
                                                                                \
    HBOOST_PP_COMMA_IF(I)                                                        \
    HBOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPLE_SIZE,1,ATTRIBUTE)(HBOOST_PP_CAT(_,I))

#define HBOOST_FUSION_DEFINE_TPL_STRUCT_CTOR_ARG_I(R, DATA, I, ATTRIBUTE)        \
                                                                                \
        HBOOST_PP_COMMA_IF(I)                                                    \
        typename hboost::call_traits<                                            \
            typename hboost::fusion::detail::get_first_arg<                      \
                HBOOST_PP_TUPLE_ELEM(                                            \
                    HBOOST_PP_TUPLE_ELEM(3,0,DATA),                              \
                    0,                                                          \
                    ATTRIBUTE)                                                  \
              , HBOOST_PP_TUPLE_ELEM(3,2,DATA)                                   \
            >::type                                                             \
        >::param_type HBOOST_PP_CAT(_,I)

#define HBOOST_FUSION_DEFINE_TPL_STRUCT_CTOR_N(                                  \
    TEMPLATE_PARAMS_SEQ, NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)            \
                                                                                \
        HBOOST_FUSION_GPU_ENABLED                                                \
        NAME(HBOOST_PP_SEQ_FOR_EACH_I_R(                                         \
                1,                                                              \
                HBOOST_FUSION_DEFINE_TPL_STRUCT_CTOR_ARG_I,                      \
                (                                                               \
                    ATTRIBUTE_TUPLE_SIZE,                                       \
                    HBOOST_PP_SEQ_SIZE(ATTRIBUTES_SEQ),                          \
                    HBOOST_PP_SEQ_HEAD(TEMPLATE_PARAMS_SEQ)                      \
                ),                                                              \
                ATTRIBUTES_SEQ))                                                \
          : HBOOST_PP_SEQ_FOR_EACH_I_R(                                          \
                1,                                                              \
                HBOOST_FUSION_DEFINE_STRUCT_CTOR_FILLER_I,                       \
                ATTRIBUTE_TUPLE_SIZE,                                           \
                ATTRIBUTES_SEQ)                                                 \
        {}

#define HBOOST_FUSION_DEFINE_STRUCT_CTOR_ARG_I(                                  \
        R, ATTRIBUTE_TUPLE_SIZE, I, ATTRIBUTE)                                  \
                                                                                \
        HBOOST_PP_COMMA_IF(I)                                                    \
        hboost::call_traits<                                                     \
            HBOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPLE_SIZE,0,ATTRIBUTE)               \
        >::param_type HBOOST_PP_CAT(_,I)

#define HBOOST_FUSION_DEFINE_STRUCT_CTOR_N(                                      \
    NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)                                 \
                                                                                \
        HBOOST_FUSION_GPU_ENABLED                                \
        NAME(HBOOST_PP_SEQ_FOR_EACH_I_R(                                         \
                1,                                                              \
                HBOOST_FUSION_DEFINE_STRUCT_CTOR_ARG_I,                          \
                ATTRIBUTE_TUPLE_SIZE,                                           \
                ATTRIBUTES_SEQ))                                                \
          : HBOOST_PP_SEQ_FOR_EACH_I_R(                                          \
                1,                                                              \
                HBOOST_FUSION_DEFINE_STRUCT_CTOR_FILLER_I,                       \
                ATTRIBUTE_TUPLE_SIZE,                                           \
                ATTRIBUTES_SEQ)                                                 \
        {}

#define HBOOST_FUSION_DEFINE_STRUCT_CTOR(                                        \
    NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)                                 \
                                                                                \
    HBOOST_PP_IF(HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(ATTRIBUTES_SEQ)),                \
        HBOOST_FUSION_DEFINE_STRUCT_CTOR_N,                                      \
        HBOOST_FUSION_DEFINE_STRUCT_CTOR_1)(                                     \
            NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)

#define HBOOST_FUSION_DEFINE_TPL_STRUCT_CTOR(                                    \
    TEMPLATE_PARAMS_SEQ, NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)            \
                                                                                \
    HBOOST_PP_IF(HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(ATTRIBUTES_SEQ)),                \
        HBOOST_FUSION_DEFINE_TPL_STRUCT_CTOR_N,                                  \
        HBOOST_FUSION_DEFINE_TPL_STRUCT_CTOR_1)(                                 \
            TEMPLATE_PARAMS_SEQ, NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)

#define HBOOST_FUSION_DEFINE_NONEMPTY_STRUCT_IMPL(                               \
    NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)                                 \
                                                                                \
        HBOOST_FUSION_DEFINE_STRUCT_IMPL_IMPL(                                   \
            NAME, HBOOST_PP_SEQ_TAIL(ATTRIBUTES_SEQ), ATTRIBUTE_TUPLE_SIZE)      \
                                                                                \
        HBOOST_FUSION_DEFINE_STRUCT_CTOR(                                        \
            NAME, HBOOST_PP_SEQ_TAIL(ATTRIBUTES_SEQ), ATTRIBUTE_TUPLE_SIZE)

#define HBOOST_FUSION_DEFINE_EMPTY_STRUCT_IMPL(                                  \
    NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)                                 \
                                                                                \
        template<typename Seq>                                                  \
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED                                \
        NAME(Seq const&)                                                        \
        {}                                                                      \
                                                                                \
        template<typename Seq>                                                  \
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED                          \
        self_type&                                                              \
        operator=(Seq const& seq)                                               \
        {                                                                       \
            return *this;                                                       \
        }

#define HBOOST_FUSION_DEFINE_STRUCT_IMPL(                                        \
    NAMESPACE_SEQ, NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)                  \
                                                                                \
    HBOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DEFINITION_BEGIN(NAMESPACE_SEQ)         \
                                                                                \
    struct NAME                                                                 \
    {                                                                           \
        typedef NAME self_type;                                                 \
                                                                                \
        HBOOST_PP_IF(HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(ATTRIBUTES_SEQ)),            \
            HBOOST_FUSION_DEFINE_NONEMPTY_STRUCT_IMPL,                           \
            HBOOST_FUSION_DEFINE_EMPTY_STRUCT_IMPL)(                             \
                NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)                     \
    };                                                                          \
                                                                                \
    HBOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DEFINITION_END(NAMESPACE_SEQ)

#define HBOOST_FUSION_DEFINE_NONEMPTY_TPL_STRUCT_IMPL(                           \
    TEMPLATE_PARAMS_SEQ, NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)            \
                                                                                \
        HBOOST_FUSION_DEFINE_STRUCT_IMPL_IMPL(                                   \
            NAME, HBOOST_PP_SEQ_TAIL(ATTRIBUTES_SEQ), ATTRIBUTE_TUPLE_SIZE)      \
                                                                                \
        HBOOST_FUSION_DEFINE_TPL_STRUCT_CTOR(                                    \
            TEMPLATE_PARAMS_SEQ,                                                \
            NAME,                                                               \
            HBOOST_PP_SEQ_TAIL(ATTRIBUTES_SEQ),                                  \
            ATTRIBUTE_TUPLE_SIZE)

#define HBOOST_FUSION_DEFINE_EMPTY_TPL_STRUCT_IMPL(                              \
    TEMPLATE_PARAMS_SEQ, NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)            \
                                                                                \
        HBOOST_FUSION_DEFINE_EMPTY_STRUCT_IMPL(                                  \
            NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)

#define HBOOST_FUSION_DEFINE_TPL_STRUCT_IMPL(                                    \
    TEMPLATE_PARAMS_SEQ,                                                        \
    NAMESPACE_SEQ,                                                              \
    NAME,                                                                       \
    ATTRIBUTES_SEQ,                                                             \
    ATTRIBUTE_TUPLE_SIZE)                                                       \
                                                                                \
    HBOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DEFINITION_BEGIN(NAMESPACE_SEQ)         \
                                                                                \
    template<                                                                   \
        HBOOST_FUSION_ADAPT_STRUCT_UNPACK_TEMPLATE_PARAMS_IMPL(                  \
            (0)TEMPLATE_PARAMS_SEQ)                                             \
    >                                                                           \
    struct NAME                                                                 \
    {                                                                           \
        typedef NAME self_type;                                                 \
                                                                                \
        HBOOST_PP_IF(HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(ATTRIBUTES_SEQ)),            \
            HBOOST_FUSION_DEFINE_NONEMPTY_TPL_STRUCT_IMPL,                       \
            HBOOST_FUSION_DEFINE_EMPTY_TPL_STRUCT_IMPL)(                         \
                TEMPLATE_PARAMS_SEQ, NAME, ATTRIBUTES_SEQ, ATTRIBUTE_TUPLE_SIZE)\
    };                                                                          \
                                                                                \
    HBOOST_FUSION_ADAPT_STRUCT_NAMESPACE_DEFINITION_END(NAMESPACE_SEQ)

namespace hboost { namespace fusion { namespace detail
{
    template<typename A1, typename A2>
    struct get_first_arg
    {
        typedef A1 type;
    };
}}}

#endif
