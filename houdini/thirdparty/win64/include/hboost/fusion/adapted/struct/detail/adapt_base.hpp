/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden
    Copyright (c) 2009-2011 Christopher Schmidt
    Copyright (c) 2013-2014 Damien Buhl

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HBOOST_FUSION_ADAPTED_STRUCT_DETAIL_ADAPT_BASE_HPP
#define HBOOST_FUSION_ADAPTED_STRUCT_DETAIL_ADAPT_BASE_HPP

#include <hboost/fusion/support/config.hpp>
#include <hboost/config.hpp>
#include <hboost/fusion/support/tag_of_fwd.hpp>
#include <hboost/fusion/adapted/struct/detail/adapt_auto.hpp>
#include <hboost/fusion/adapted/struct/detail/adapt_is_tpl.hpp>

#include <hboost/preprocessor/empty.hpp>
#include <hboost/preprocessor/stringize.hpp>
#include <hboost/preprocessor/control/if.hpp>
#include <hboost/preprocessor/seq/size.hpp>
#include <hboost/preprocessor/seq/for_each.hpp>
#include <hboost/preprocessor/seq/for_each_i.hpp>
#include <hboost/preprocessor/seq/enum.hpp>
#include <hboost/preprocessor/seq/seq.hpp>
#include <hboost/preprocessor/tuple/eat.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/arithmetic/dec.hpp>
#include <hboost/preprocessor/comparison/less.hpp>
#include <hboost/preprocessor/logical/not.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/tag.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/type_traits/add_const.hpp>

#include <hboost/typeof/typeof.hpp>


#define HBOOST_FUSION_ADAPT_STRUCT_UNPACK_NAME_TEMPLATE_PARAMS(SEQ)              \
    HBOOST_PP_SEQ_HEAD(SEQ)<HBOOST_PP_SEQ_ENUM(HBOOST_PP_SEQ_TAIL(SEQ))>           \
    HBOOST_PP_EMPTY()

#define HBOOST_FUSION_ADAPT_STRUCT_UNPACK_NAME(SEQ)                              \
    HBOOST_PP_IF(                                                                \
        HBOOST_PP_SEQ_HEAD(SEQ),                                                 \
        HBOOST_FUSION_ADAPT_STRUCT_UNPACK_NAME_TEMPLATE_PARAMS,                  \
        HBOOST_PP_SEQ_HEAD)(HBOOST_PP_SEQ_TAIL(SEQ))

#define HBOOST_FUSION_ADAPT_STRUCT_UNPACK_TEMPLATE_PARAMS_IMPL_C(R, _, ELEM)     \
    (typename ELEM)
#define HBOOST_FUSION_ADAPT_STRUCT_UNPACK_TEMPLATE_PARAMS_IMPL(SEQ)              \
    HBOOST_PP_SEQ_ENUM(                                                          \
        HBOOST_PP_SEQ_FOR_EACH(                                                  \
            HBOOST_FUSION_ADAPT_STRUCT_UNPACK_TEMPLATE_PARAMS_IMPL_C,            \
            _,                                                                  \
            HBOOST_PP_SEQ_TAIL(SEQ)))
#define HBOOST_FUSION_ADAPT_STRUCT_UNPACK_TEMPLATE_PARAMS(SEQ)                   \
    HBOOST_PP_IF(                                                                \
        HBOOST_PP_SEQ_HEAD(SEQ),                                                 \
        HBOOST_FUSION_ADAPT_STRUCT_UNPACK_TEMPLATE_PARAMS_IMPL,                  \
        HBOOST_PP_TUPLE_EAT(1))(SEQ)

#ifdef HBOOST_MSVC
#   define HBOOST_FUSION_ATTRIBUTE_TYPEOF(                                       \
        NAME_SEQ, ATTRIBUTE, ATTRIBUTE_TUPLE_SIZE, PREFIX, TEMPLATE_PARAMS_SEQ) \
                                                                                \
    HBOOST_FUSION_ADAPT_STRUCT_MSVC_REDEFINE_TEMPLATE_PARAMS(                    \
        TEMPLATE_PARAMS_SEQ)                                                    \
                                                                                \
    struct deduced_attr_type {                                                  \
      static const HBOOST_FUSION_ADAPT_STRUCT_UNPACK_NAME(NAME_SEQ)& obj;        \
      typedef                                                                   \
      HBOOST_PP_IF(HBOOST_FUSION_ADAPT_IS_TPL(TEMPLATE_PARAMS_SEQ), typename, )   \
      HBOOST_TYPEOF( PREFIX() obj.HBOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPLE_SIZE,      \
            0, ATTRIBUTE))                                                      \
      type;                                                                     \
    };                                                                          \
                                                                                \
    typedef                                                                     \
        HBOOST_PP_IF(HBOOST_FUSION_ADAPT_IS_TPL(TEMPLATE_PARAMS_SEQ), typename, ) \
        deduced_attr_type::type attribute_type;

#else 
#   define HBOOST_FUSION_ATTRIBUTE_TYPEOF(                                       \
        NAME_SEQ, ATTRIBUTE, ATTRIBUTE_TUPLE_SIZE, PREFIX, TEMPLATE_PARAMS_SEQ) \
                                                                                \
    struct deduced_attr_type {                                                  \
      static const HBOOST_FUSION_ADAPT_STRUCT_UNPACK_NAME(NAME_SEQ)& obj;        \
      typedef HBOOST_TYPEOF(                                                     \
          PREFIX() obj.HBOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPLE_SIZE, 0, ATTRIBUTE)) \
      type;                                                                     \
    };                                                                          \
                                                                                \
    typedef                                                                     \
        HBOOST_PP_IF(HBOOST_FUSION_ADAPT_IS_TPL(TEMPLATE_PARAMS_SEQ), typename, ) \
        deduced_attr_type::type attribute_type;

#endif

#define HBOOST_FUSION_ATTRIBUTE_GIVENTYPE(                                       \
    NAME_SEQ, ATTRIBUTE, ATTRIBUTE_TUPLE_SIZE, PREFIX, TEMPLATE_PARAMS_SEQ)     \
    typedef                                                                     \
        HBOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPLE_SIZE, 0, ATTRIBUTE) attribute_type;
   

#ifdef HBOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS
#   define HBOOST_FUSION_ADAPT_STRUCT_TAG_OF_SPECIALIZATION(                     \
        MODIFIER, TEMPLATE_PARAMS_SEQ, NAME_SEQ, TAG)                           \
                                                                                \
    template<                                                                   \
        HBOOST_FUSION_ADAPT_STRUCT_UNPACK_TEMPLATE_PARAMS(TEMPLATE_PARAMS_SEQ)   \
    >                                                                           \
    struct tag_of<                                                              \
        HBOOST_FUSION_ADAPT_STRUCT_UNPACK_NAME(NAME_SEQ) MODIFIER                \
      , void                                                                    \
    >                                                                           \
    {                                                                           \
        typedef TAG type;                                                       \
    };
#else
#   define HBOOST_FUSION_ADAPT_STRUCT_TAG_OF_SPECIALIZATION(                     \
        MODIFIER, TEMPLATE_PARAMS_SEQ, NAME_SEQ, TAG)                           \
                                                                                \
    template<                                                                   \
        HBOOST_FUSION_ADAPT_STRUCT_UNPACK_TEMPLATE_PARAMS(TEMPLATE_PARAMS_SEQ)   \
    >                                                                           \
    struct tag_of<HBOOST_FUSION_ADAPT_STRUCT_UNPACK_NAME(NAME_SEQ) MODIFIER>     \
    {                                                                           \
        typedef TAG type;                                                       \
    };
#endif

#define HBOOST_FUSION_ADAPT_STRUCT_BASE_UNPACK_AND_CALL(R,DATA,I,ATTRIBUTE)      \
    HBOOST_PP_TUPLE_ELEM(4,0,DATA)(                                              \
        HBOOST_PP_TUPLE_ELEM(4,1,DATA),                                          \
        HBOOST_PP_TUPLE_ELEM(4,2,DATA),                                          \
        HBOOST_PP_TUPLE_ELEM(4,3,DATA),                                          \
        I,                                                                      \
        ATTRIBUTE)

#ifdef HBOOST_MSVC
#   define HBOOST_FUSION_ADAPT_STRUCT_MSVC_REDEFINE_TEMPLATE_PARAM(R,_,ELEM)     \
        typedef ELEM ELEM;
#   define HBOOST_FUSION_ADAPT_STRUCT_MSVC_REDEFINE_TEMPLATE_PARAMS_IMPL(SEQ)    \
        HBOOST_PP_SEQ_FOR_EACH(                                                  \
            HBOOST_FUSION_ADAPT_STRUCT_MSVC_REDEFINE_TEMPLATE_PARAM,             \
            _,                                                                  \
            HBOOST_PP_SEQ_TAIL(SEQ))
#   define HBOOST_FUSION_ADAPT_STRUCT_MSVC_REDEFINE_TEMPLATE_PARAMS(SEQ)         \
        HBOOST_PP_IF(                                                            \
            HBOOST_PP_SEQ_HEAD(SEQ),                                             \
            HBOOST_FUSION_ADAPT_STRUCT_MSVC_REDEFINE_TEMPLATE_PARAMS_IMPL,       \
            HBOOST_PP_TUPLE_EAT(1))(SEQ)
#else
#   define HBOOST_FUSION_ADAPT_STRUCT_MSVC_REDEFINE_TEMPLATE_PARAMS(SEQ)
#endif

#define HBOOST_FUSION_ADAPT_STRUCT_C_BASE(                                       \
    TEMPLATE_PARAMS_SEQ,NAME_SEQ,IS_VIEW,                                       \
    I,PREFIX,ATTRIBUTE,ATTRIBUTE_TUPLE_SIZE,                                    \
    DEDUCE_TYPE)                                                                \
                                                                                \
    template<                                                                   \
        HBOOST_FUSION_ADAPT_STRUCT_UNPACK_TEMPLATE_PARAMS(TEMPLATE_PARAMS_SEQ)   \
    >                                                                           \
    struct access::struct_member<                                               \
        HBOOST_FUSION_ADAPT_STRUCT_UNPACK_NAME(NAME_SEQ)                         \
      , I                                                                       \
    >                                                                           \
    {                                                                           \
        HBOOST_PP_IF(DEDUCE_TYPE,                                                \
            HBOOST_FUSION_ATTRIBUTE_TYPEOF, HBOOST_FUSION_ATTRIBUTE_GIVENTYPE)(   \
                NAME_SEQ,                                                       \
                ATTRIBUTE,                                                      \
                ATTRIBUTE_TUPLE_SIZE,                                           \
                PREFIX,                                                         \
                TEMPLATE_PARAMS_SEQ)                                            \
                                                                                \
        HBOOST_FUSION_ADAPT_STRUCT_MSVC_REDEFINE_TEMPLATE_PARAMS(                \
            TEMPLATE_PARAMS_SEQ)                                                \
                                                                                \
        typedef attribute_type type;                                            \
                                                                                \
        template<typename Seq>                                                  \
        struct apply                                                            \
        {                                                                       \
            typedef typename                                                    \
                add_reference<                                                  \
                    typename mpl::eval_if<                                      \
                        is_const<Seq>                                           \
                      , add_const<attribute_type>                               \
                      , mpl::identity<attribute_type>                           \
                    >::type                                                     \
                >::type                                                         \
            type;                                                               \
                                                                                \
            HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED                            \
            static type                                                         \
            call(Seq& seq)                                                      \
            {                                                                   \
                return seq.PREFIX()                                             \
                    HBOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPLE_SIZE,                   \
                        HBOOST_PP_NOT(DEDUCE_TYPE), ATTRIBUTE);                  \
            }                                                                   \
        };                                                                      \
    };                                                                          \
                                                                                \
    template<                                                                   \
        HBOOST_FUSION_ADAPT_STRUCT_UNPACK_TEMPLATE_PARAMS(TEMPLATE_PARAMS_SEQ)   \
    >                                                                           \
    struct struct_member_name<                                                  \
        HBOOST_FUSION_ADAPT_STRUCT_UNPACK_NAME(NAME_SEQ)                         \
      , I                                                                       \
    >                                                                           \
    {                                                                           \
        typedef char const* type;                                               \
                                                                                \
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED                                \
        static type                                                             \
        call()                                                                  \
        {                                                                       \
            return HBOOST_PP_STRINGIZE(                                          \
               HBOOST_PP_TUPLE_ELEM(ATTRIBUTE_TUPLE_SIZE,                        \
                   HBOOST_PP_NOT(DEDUCE_TYPE), ATTRIBUTE));                      \
        }                                                                       \
    };

#define HBOOST_FUSION_ADAPT_STRUCT_BASE(                                         \
    TEMPLATE_PARAMS_SEQ,                                                        \
    NAME_SEQ,                                                                   \
    TAG,                                                                        \
    IS_VIEW,                                                                    \
    ATTRIBUTES_SEQ,                                                             \
    ATTRIBUTES_CALLBACK)                                                        \
                                                                                \
namespace hboost                                                                 \
{                                                                               \
    namespace fusion                                                            \
    {                                                                           \
        namespace traits                                                        \
        {                                                                       \
            HBOOST_FUSION_ADAPT_STRUCT_TAG_OF_SPECIALIZATION(                    \
                HBOOST_PP_EMPTY(), TEMPLATE_PARAMS_SEQ, NAME_SEQ, TAG)           \
            HBOOST_FUSION_ADAPT_STRUCT_TAG_OF_SPECIALIZATION(                    \
                const, TEMPLATE_PARAMS_SEQ, NAME_SEQ, TAG)                      \
        }                                                                       \
                                                                                \
        namespace extension                                                     \
        {                                                                       \
            HBOOST_PP_IF(                                                        \
                HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(ATTRIBUTES_SEQ)),                \
                HBOOST_PP_SEQ_FOR_EACH_I_R,                                      \
                HBOOST_PP_TUPLE_EAT(4))(                                         \
                    1,                                                          \
                    HBOOST_FUSION_ADAPT_STRUCT_BASE_UNPACK_AND_CALL,             \
                    (ATTRIBUTES_CALLBACK,TEMPLATE_PARAMS_SEQ,NAME_SEQ, IS_VIEW),\
                    HBOOST_PP_SEQ_TAIL(ATTRIBUTES_SEQ))                          \
                                                                                \
            template<                                                           \
                HBOOST_FUSION_ADAPT_STRUCT_UNPACK_TEMPLATE_PARAMS(               \
                    TEMPLATE_PARAMS_SEQ)                                        \
            >                                                                   \
            struct struct_size<HBOOST_FUSION_ADAPT_STRUCT_UNPACK_NAME(NAME_SEQ)> \
              : mpl::int_<HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(ATTRIBUTES_SEQ))>      \
            {};                                                                 \
                                                                                \
            template<                                                           \
                HBOOST_FUSION_ADAPT_STRUCT_UNPACK_TEMPLATE_PARAMS(               \
                    TEMPLATE_PARAMS_SEQ)                                        \
            >                                                                   \
            struct struct_is_view<                                              \
                HBOOST_FUSION_ADAPT_STRUCT_UNPACK_NAME(NAME_SEQ)                 \
            >                                                                   \
              : mpl::HBOOST_PP_IIF(IS_VIEW,true_,false_)                         \
            {};                                                                 \
        }                                                                       \
    }                                                                           \
                                                                                \
    namespace mpl                                                               \
    {                                                                           \
        template<typename>                                                      \
        struct sequence_tag;                                                    \
                                                                                \
        template<                                                               \
            HBOOST_FUSION_ADAPT_STRUCT_UNPACK_TEMPLATE_PARAMS(                   \
                TEMPLATE_PARAMS_SEQ)                                            \
        >                                                                       \
        struct sequence_tag<HBOOST_FUSION_ADAPT_STRUCT_UNPACK_NAME(NAME_SEQ)>    \
        {                                                                       \
            typedef fusion::fusion_sequence_tag type;                           \
        };                                                                      \
                                                                                \
        template<                                                               \
            HBOOST_FUSION_ADAPT_STRUCT_UNPACK_TEMPLATE_PARAMS(                   \
                TEMPLATE_PARAMS_SEQ)                                            \
        >                                                                       \
        struct sequence_tag<                                                    \
            HBOOST_FUSION_ADAPT_STRUCT_UNPACK_NAME(NAME_SEQ) const               \
        >                                                                       \
        {                                                                       \
            typedef fusion::fusion_sequence_tag type;                           \
        };                                                                      \
    }                                                                           \
}

#endif
