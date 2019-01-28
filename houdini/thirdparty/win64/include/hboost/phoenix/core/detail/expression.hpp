/*=============================================================================
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HBOOST_PHOENIX_CORE_DETAIL_EXPRESSION_HPP
#define HBOOST_PHOENIX_CORE_DETAIL_EXPRESSION_HPP

#include <hboost/preprocessor/empty.hpp>
#include <hboost/preprocessor/arithmetic/add.hpp>
#include <hboost/preprocessor/arithmetic/dec.hpp>
#include <hboost/preprocessor/comma_if.hpp>
#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/seq/size.hpp>
#include <hboost/preprocessor/seq/enum.hpp>
#include <hboost/preprocessor/seq/for_each.hpp>
#include <hboost/preprocessor/seq/pop_back.hpp>
#include <hboost/preprocessor/seq/reverse.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/enum_params.hpp>
#include <hboost/preprocessor/repeat_from_to.hpp>

#define HBOOST_PHOENIX_DEFINE_EXPRESSION(NAME_SEQ, SEQ)                          \
    HBOOST_PHOENIX_DEFINE_EXPRESSION_BASE(                                       \
        NAME_SEQ                                                                \
      , SEQ                                                                     \
      , HBOOST_PHOENIX_DEFINE_EXPRESSION_EXPRESSION_DEFAULT                      \
      , HBOOST_PHOENIX_DEFINE_EXPRESSION_RULE_DEFAULT                            \
      , HBOOST_PHOENIX_DEFINE_EXPRESSION_RESULT_OF_MAKE_DEFAULT                  \
      , HBOOST_PHOENIX_DEFINE_EXPRESSION_MAKE_EXPRESSION_DEFAULT                 \
      , _                                                                       \
    )                                                                           \
/**/

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_VARARG(NAME_SEQ, GRAMMAR_SEQ, LIMIT)    \
    HBOOST_PHOENIX_DEFINE_EXPRESSION_BASE(                                       \
        NAME_SEQ                                                                \
      , GRAMMAR_SEQ                                                             \
      , HBOOST_PHOENIX_DEFINE_EXPRESSION_EXPRESSION_VARARG                       \
      , HBOOST_PHOENIX_DEFINE_EXPRESSION_RULE_VARARG                             \
      , HBOOST_PHOENIX_DEFINE_EXPRESSION_RESULT_OF_MAKE_VARARG                   \
      , HBOOST_PHOENIX_DEFINE_EXPRESSION_MAKE_EXPRESSION_VARARG                  \
      , LIMIT                                                                   \
    )                                                                           \
/**/

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_EXT(ACTOR, NAME_SEQ, GRAMMAR_SEQ)       \
    HBOOST_PHOENIX_DEFINE_EXPRESSION_BASE(                                       \
        NAME_SEQ                                                                \
      , GRAMMAR_SEQ                                                             \
      , HBOOST_PHOENIX_DEFINE_EXPRESSION_EXPRESSION_EXT                          \
      , HBOOST_PHOENIX_DEFINE_EXPRESSION_RULE_DEFAULT                            \
      , HBOOST_PHOENIX_DEFINE_EXPRESSION_RESULT_OF_MAKE_DEFAULT                  \
      , HBOOST_PHOENIX_DEFINE_EXPRESSION_MAKE_EXPRESSION_DEFAULT                 \
      , ACTOR                                                                   \
    )                                                                           \
/**/

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_EXT_VARARG(ACTOR, NAME, GRAMMAR, LIMIT) \
    HBOOST_PHOENIX_DEFINE_EXPRESSION_BASE(                                       \
        NAME_SEQ                                                                \
      , GRAMMAR_SEQ                                                             \
      , HBOOST_PHOENIX_DEFINE_EXPRESSION_EXPRESSION_VARARG_EXT                   \
      , HBOOST_PHOENIX_DEFINE_EXPRESSION_RULE_VARARG                             \
      , HBOOST_PHOENIX_DEFINE_EXPRESSION_RESULT_OF_MAKE_VARARG                   \
      , HBOOST_PHOENIX_DEFINE_EXPRESSION_MAKE_EXPRESSION_VARARG                  \
      , ACTOR                                                                   \
    )                                                                           \
/**/

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_NAMESPACE(R, D, E)                      \
namespace E {                                                                   \
/**/

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_NAMESPACE_END(R, D, E)                  \
}                                                                               \
/**/

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_NS(R, D, E)                             \
E ::                                                                            \
/**/

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_BASE(NAME_SEQ, GRAMMAR_SEQ, EXPRESSION, RULE, RESULT_OF_MAKE, MAKE_EXPRESSION, DATA) \
HBOOST_PP_SEQ_FOR_EACH(                                                          \
    HBOOST_PHOENIX_DEFINE_EXPRESSION_NAMESPACE                                   \
  , _                                                                           \
  , HBOOST_PP_SEQ_POP_BACK(NAME_SEQ)                                             \
)                                                                               \
    namespace tag                                                               \
    {                                                                           \
        struct HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ)) {};            \
        template <typename Ostream>                                             \
        inline Ostream &operator<<(                                             \
            Ostream & os                                                        \
          , HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ)))                  \
        {                                                                       \
            os << HBOOST_PP_STRINGIZE(                                           \
                HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))               \
            );                                                                  \
            return os;                                                          \
        }                                                                       \
    }                                                                           \
    namespace expression                                                        \
    {                                                                           \
        EXPRESSION(NAME_SEQ, GRAMMAR_SEQ, DATA)                                 \
    }                                                                           \
    namespace rule                                                              \
    {                                                                           \
        RULE(NAME_SEQ, GRAMMAR_SEQ, DATA)                                       \
    }                                                                           \
    namespace functional                                                        \
    {                                                                           \
        typedef                                                                 \
            hboost::proto::functional::make_expr<                                \
                    tag:: HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))     \
            >                                                                   \
            HBOOST_PP_CAT(                                                       \
                make_                                                           \
              , HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))               \
            );                                                                  \
    }                                                                           \
    namespace result_of                                                         \
    {                                                                           \
        RESULT_OF_MAKE(NAME_SEQ, GRAMMAR_SEQ, DATA)                             \
    }                                                                           \
    MAKE_EXPRESSION(NAME_SEQ, GRAMMAR_SEQ, DATA)                                \
                                                                                \
HBOOST_PP_SEQ_FOR_EACH(                                                          \
    HBOOST_PHOENIX_DEFINE_EXPRESSION_NAMESPACE_END                               \
  , _                                                                           \
  , HBOOST_PP_SEQ_POP_BACK(NAME_SEQ)                                             \
)                                                                               \
namespace hboost { namespace hboostphoenix                                             \
{                                                                               \
    template <typename Dummy>                                                   \
    struct meta_grammar::case_<                                                 \
        :: HBOOST_PP_SEQ_FOR_EACH(                                               \
            HBOOST_PHOENIX_DEFINE_EXPRESSION_NS                                  \
          , _                                                                   \
          , HBOOST_PP_SEQ_POP_BACK(NAME_SEQ)                                     \
        ) tag:: HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))               \
      , Dummy                                                                   \
    >                                                                           \
        : enable_rule<                                                          \
            :: HBOOST_PP_SEQ_FOR_EACH(                                           \
                HBOOST_PHOENIX_DEFINE_EXPRESSION_NS                              \
              , _                                                               \
              , HBOOST_PP_SEQ_POP_BACK(NAME_SEQ)                                 \
            ) rule:: HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))          \
         , Dummy                                                                \
        >                                                                       \
    {};                                                                         \
} }                                                                             \
/**/

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_EXPRESSION_DEFAULT(NAME_SEQ, GRAMMAR_SEQ, D) \
        template <HBOOST_PHOENIX_typename_A(HBOOST_PP_SEQ_SIZE(GRAMMAR_SEQ))>     \
        struct HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))                \
            : hboost::hboostphoenix::expr<                                             \
                :: HBOOST_PP_SEQ_FOR_EACH(                                       \
                    HBOOST_PHOENIX_DEFINE_EXPRESSION_NS                          \
                  , _                                                           \
                  , HBOOST_PP_SEQ_POP_BACK(NAME_SEQ)                             \
                ) tag:: HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))       \
              , HBOOST_PP_ENUM_PARAMS(HBOOST_PP_SEQ_SIZE(GRAMMAR_SEQ), A)>        \
        {};                                                                     \
/**/
        
#define HBOOST_PHOENIX_DEFINE_EXPRESSION_RULE_DEFAULT(NAME_SEQ, GRAMMAR_SEQ, D)  \
    struct HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))                    \
        : expression:: HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))        \
            <HBOOST_PP_SEQ_ENUM(GRAMMAR_SEQ)>                                    \
    {};                                                                         \
/**/

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_RESULT_OF_MAKE_DEFAULT(NAME_SEQ, GRAMMAR_SEQ, D) \
    template <HBOOST_PHOENIX_typename_A(HBOOST_PP_SEQ_SIZE(GRAMMAR_SEQ))>         \
    struct HBOOST_PP_CAT(make_, HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))) \
        : hboost::result_of<                                                     \
            functional::                                                        \
                HBOOST_PP_CAT(                                                   \
                    make_                                                       \
                  , HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))           \
                )(HBOOST_PHOENIX_A(HBOOST_PP_SEQ_SIZE(GRAMMAR_SEQ)))              \
        >                                                                       \
    {};                                                                         \
/**/

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_MAKE_EXPRESSION_DEFAULT(NAME_SEQ, GRAMMAR_SEQ, D) \
    template <HBOOST_PHOENIX_typename_A(HBOOST_PP_SEQ_SIZE(GRAMMAR_SEQ))>         \
    inline                                                                      \
    typename                                                                    \
        result_of::HBOOST_PP_CAT(                                                \
            make_                                                               \
          , HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))                   \
        )<                                                                      \
            HBOOST_PHOENIX_A(HBOOST_PP_SEQ_SIZE(GRAMMAR_SEQ))                     \
        >::type const                                                           \
    HBOOST_PP_CAT(                                                               \
        make_                                                                   \
      , HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))                       \
    )(                                                                          \
        HBOOST_PHOENIX_A_const_ref_a(HBOOST_PP_SEQ_SIZE(GRAMMAR_SEQ))             \
    )                                                                           \
    {                                                                           \
        return                                                                  \
            functional::HBOOST_PP_CAT(                                           \
                make_                                                           \
              , HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))               \
            )()(                                                                \
              HBOOST_PHOENIX_a(HBOOST_PP_SEQ_SIZE(GRAMMAR_SEQ))                   \
            );                                                                  \
    }                                                                           \
/**/

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_VARARG_R(_, N, NAME)                    \
    template <                                                                  \
        HBOOST_PHOENIX_typename_A(                                               \
            HBOOST_PP_ADD(                                                       \
                N                                                               \
              , HBOOST_PP_SEQ_SIZE(HBOOST_PP_TUPLE_ELEM(2, 1, NAME))              \
            )                                                                   \
        )                                                                       \
    >                                                                           \
    struct HBOOST_PP_TUPLE_ELEM(2, 0, NAME)<                                     \
        HBOOST_PHOENIX_A(                                                        \
            HBOOST_PP_ADD(N, HBOOST_PP_SEQ_SIZE(HBOOST_PP_TUPLE_ELEM(2, 1, NAME))) \
        )                                                                       \
    >                                                                           \
        : hboost::hboostphoenix::expr<                                                 \
            tag:: HBOOST_PP_TUPLE_ELEM(2, 0, NAME)                               \
          , HBOOST_PHOENIX_A(                                                    \
                HBOOST_PP_ADD(                                                   \
                    N                                                           \
                  , HBOOST_PP_SEQ_SIZE(HBOOST_PP_TUPLE_ELEM(2, 1, NAME))          \
                )                                                               \
            )                                                                   \
        >                                                                       \
    {};                                                                         \
/**/

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_EXPRESSION_VARARG(NAME_SEQ, GRAMMAR_SEQ, LIMIT) \
        template <                                                              \
            HBOOST_PHOENIX_typename_A_void(                                      \
                HBOOST_PP_ADD(                                                   \
                    LIMIT, HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(GRAMMAR_SEQ)))        \
            )                                                                   \
          , typename Dummy = void                                               \
        >                                                                       \
        struct HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ));               \
                                                                                \
        HBOOST_PP_REPEAT_FROM_TO(                                                \
            1                                                                   \
          , HBOOST_PP_ADD(LIMIT, HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(GRAMMAR_SEQ)))   \
          , HBOOST_PHOENIX_DEFINE_EXPRESSION_VARARG_R                            \
          , (                                                                   \
                HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))               \
              , HBOOST_PP_SEQ_POP_BACK(GRAMMAR_SEQ)                              \
            )                                                                   \
        )                                                                       \
/**/

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_RULE_VARARG(NAME_SEQ, GRAMMAR_SEQ, LIMIT) \
        struct HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))                \
            : expression:: HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ)) <  \
                HBOOST_PP_IF(                                                    \
                    HBOOST_PP_EQUAL(1, HBOOST_PP_SEQ_SIZE(GRAMMAR_SEQ))           \
                  , HBOOST_PP_EMPTY                                              \
                  , HBOOST_PP_IDENTITY(                                          \
                      HBOOST_PP_SEQ_ENUM(HBOOST_PP_SEQ_POP_BACK(GRAMMAR_SEQ))     \
                    )                                                           \
                )()                                                             \
                HBOOST_PP_COMMA_IF(HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(GRAMMAR_SEQ))) \
                hboost::proto::vararg<                                           \
                    HBOOST_PP_SEQ_ELEM(                                          \
                        HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(GRAMMAR_SEQ))            \
                      , GRAMMAR_SEQ                                             \
                    )                                                           \
                >                                                               \
            >                                                                   \
        {};                                                                     \
/**/
        
#define HBOOST_PHOENIX_DEFINE_EXPRESSION_RESULT_OF_MAKE_VARARG_R(Z, N, NAME)     \
    template <HBOOST_PHOENIX_typename_A(N)>                                      \
    struct HBOOST_PP_CAT(make_, NAME) <HBOOST_PHOENIX_A(N)>                        \
        : hboost::result_of<                                                     \
            functional:: HBOOST_PP_CAT(make_, NAME)(                              \
                HBOOST_PHOENIX_A(N)                                              \
            )                                                                   \
        >                                                                       \
    {};                                                                         \
/**/

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_RESULT_OF_MAKE_VARARG(NAME_SEQ, GRAMMAR_SEQ, LIMIT) \
    template <HBOOST_PHOENIX_typename_A_void(LIMIT), typename Dummy = void>      \
    struct HBOOST_PP_CAT(                                                        \
        make_                                                                   \
      , HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))                       \
    );                                                                          \
    HBOOST_PP_REPEAT_FROM_TO(                                                    \
        1                                                                       \
      , LIMIT                                                                   \
      , HBOOST_PHOENIX_DEFINE_EXPRESSION_RESULT_OF_MAKE_VARARG_R                 \
      , HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))                       \
    )                                                                           \
/**/

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_MAKE_EXPRESSION_VARARG_R(Z, N, NAME)    \
    template <HBOOST_PHOENIX_typename_A(N)>                                      \
    inline                                                                      \
    typename                                                                    \
        result_of:: HBOOST_PP_CAT(make_, NAME)<                                  \
            HBOOST_PHOENIX_A(N)                                                  \
        >::type                                                                 \
    HBOOST_PP_CAT(make_, NAME)(HBOOST_PHOENIX_A_const_ref_a(N))                   \
    {                                                                           \
        return functional::HBOOST_PP_CAT(make_, NAME)()(HBOOST_PHOENIX_a(N));     \
    }                                                                           \

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_MAKE_EXPRESSION_VARARG(NAME_SEQ, GRAMMAR_SEQ, LIMIT) \
    HBOOST_PP_REPEAT_FROM_TO(                                                    \
        1                                                                       \
      , LIMIT                                                                   \
      , HBOOST_PHOENIX_DEFINE_EXPRESSION_MAKE_EXPRESSION_VARARG_R                \
      , HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))                       \
    )                                                                           \
/**/

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_EXPRESSION_EXT(NAME_SEQ, GRAMMAR_SEQ, ACTOR) \
        template <HBOOST_PHOENIX_typename_A(HBOOST_PP_SEQ_SIZE(GRAMMAR_SEQ))>     \
        struct HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))                \
            : ::hboost::hboostphoenix::expr_ext<                                       \
                ACTOR                                                           \
              , tag:: HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(NAME_SEQ))         \
              , HBOOST_PHOENIX_A(HBOOST_PP_SEQ_SIZE(GRAMMAR_SEQ))>                \
        {};                                                                     \
/**/

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_EXT_VARARG_R(_, N, NAME)                \
    template <                                                                  \
        HBOOST_PHOENIX_typename_A(                                               \
            HBOOST_PP_ADD(                                                       \
                N                                                               \
              , HBOOST_PP_SEQ_SIZE(HBOOST_PP_TUPLE_ELEM(3, 1, NAME))              \
            )                                                                   \
        )                                                                       \
    >                                                                           \
    struct HBOOST_PP_TUPLE_ELEM(3, 0, NAME)<                                     \
        HBOOST_PHOENIX_A(                                                        \
            HBOOST_PP_ADD(N, HBOOST_PP_SEQ_SIZE(HBOOST_PP_TUPLE_ELEM(3, 1, NAME))) \
        )                                                                       \
    >                                                                           \
        : expr_ext<                                                             \
            HBOOST_PP_TUPLE_ELEM(3, 2, NAME)                                     \
          , tag:: HBOOST_PP_TUPLE_ELEM(3, 0, NAME)                               \
          , HBOOST_PHOENIX_A(                                                    \
                HBOOST_PP_ADD(                                                   \
                    N                                                           \
                  , HBOOST_PP_SEQ_SIZE(HBOOST_PP_TUPLE_ELEM(3, 1, NAME))          \
                )                                                               \
            )                                                                   \
        >                                                                       \
    {};                                                                         \
/**/

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_EXRPESSION_VARARG_EXT(N, G, D)          \
        template <                                                              \
            HBOOST_PHOENIX_typename_A_void(                                      \
                HBOOST_PP_ADD(LIMIT, HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(G)))         \
            )                                                                   \
          , typename Dummy = void                                               \
        >                                                                       \
        struct HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(N));                      \
                                                                                \
        HBOOST_PP_REPEAT_FROM_TO(                                                \
            1                                                                   \
          , HBOOST_PP_ADD(LIMIT, HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(G)))             \
          , HBOOST_PHOENIX_DEFINE_EXPRESSION_EXT_VARARG_R                        \
          , (                                                                   \
              HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(N))                        \
            , HBOOST_PP_SEQ_POP_BACK(G)                                          \
            , ACTOR                                                             \
          )                                                                     \
        )                                                                       \
/**/

#define HBOOST_PHOENIX_DEFINE_EXPRESSION_RULE_VARARG_EXT(N, GRAMMAR, D)          \
        struct HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(N))                       \
            : expression:: HBOOST_PP_SEQ_HEAD(HBOOST_PP_SEQ_REVERSE(N)) <         \
                HBOOST_PP_IF(                                                    \
                    HBOOST_PP_EQUAL(1, HBOOST_PP_SEQ_SIZE(GRAMMAR))               \
                  , HBOOST_PP_EMPTY                                              \
                  , HBOOST_PP_IDENTITY(                                          \
                      HBOOST_PP_SEQ_ENUM(HBOOST_PP_SEQ_POP_BACK(GRAMMAR))         \
                    )                                                           \
                )()                                                             \
                HBOOST_PP_COMMA_IF(HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(GRAMMAR)))     \
                proto::vararg<                                                  \
                    HBOOST_PP_SEQ_ELEM(                                          \
                        HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(GRAMMAR))                \
                      , GRAMMAR                                                 \
                    )                                                           \
                >                                                               \
            >                                                                   \
        {};                                                                     \


#endif
