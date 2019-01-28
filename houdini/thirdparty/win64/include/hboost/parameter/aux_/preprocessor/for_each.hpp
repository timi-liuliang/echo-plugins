// Copyright Daniel Wallin 2005. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PARAMETER_FOR_EACH_051217_HPP
# define HBOOST_PARAMETER_FOR_EACH_051217_HPP

# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/detail/split.hpp>
# include <hboost/preprocessor/logical/not.hpp>
# include <hboost/preprocessor/facilities/is_empty.hpp>
# include <hboost/preprocessor/tuple/eat.hpp>
# include <hboost/preprocessor/arithmetic/inc.hpp>
# include <hboost/preprocessor/repeat.hpp>
# include <hboost/preprocessor/punctuation/comma_if.hpp>
# include <hboost/preprocessor/for.hpp>
# include <hboost/preprocessor/repetition/deduce_r.hpp>

# define HBOOST_PARAMETER_FOR_EACH_head_aux2(x,y) (x,y), ~
# define HBOOST_PARAMETER_FOR_EACH_head_aux3(x,y,z) (x,y,z), ~
# define HBOOST_PARAMETER_FOR_EACH_head_aux4(x,y,z,u) (x,y,z,u), ~
# define HBOOST_PARAMETER_FOR_EACH_head(n,x) \
    HBOOST_PP_SPLIT(0, HBOOST_PP_CAT(HBOOST_PARAMETER_FOR_EACH_head_aux,n) x)

# define HBOOST_PARAMETER_FOR_EACH_pred_aux_HBOOST_PARAMETER_FOR_EACH_END_SENTINEL
# define HBOOST_PARAMETER_FOR_EACH_pred_aux_check(x) \
    HBOOST_PP_NOT(HBOOST_PP_IS_EMPTY( \
        HBOOST_PP_CAT(HBOOST_PARAMETER_FOR_EACH_pred_aux_, x) \
    )), ~

# define HBOOST_PARAMETER_FOR_EACH_pred_aux2(x,y) \
    HBOOST_PARAMETER_FOR_EACH_pred_aux_check(x)
# define HBOOST_PARAMETER_FOR_EACH_pred_aux3(x,y,z) \
    HBOOST_PARAMETER_FOR_EACH_pred_aux_check(x)
# define HBOOST_PARAMETER_FOR_EACH_pred_aux4(x,y,z,u) \
    HBOOST_PARAMETER_FOR_EACH_pred_aux_check(x)

# define HBOOST_PARAMETER_FOR_EACH_pred_aux0(n,x) \
    HBOOST_PP_CAT(HBOOST_PARAMETER_FOR_EACH_pred_aux,n) x

# if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MSVC()
#  define HBOOST_PARAMETER_FOR_EACH_pred_SPLIT_FIRST(x) \
    HBOOST_PP_SPLIT(0, x)

#  define HBOOST_PARAMETER_FOR_EACH_pred(r, state) \
    HBOOST_PARAMETER_FOR_EACH_pred_SPLIT_FIRST( \
        HBOOST_PARAMETER_FOR_EACH_pred_aux0( \
            HBOOST_PP_TUPLE_ELEM(5,3,state) \
          , HBOOST_PP_TUPLE_ELEM(5,0,state) \
        ) \
    )
# else
#  define HBOOST_PARAMETER_FOR_EACH_pred(r, state) \
    HBOOST_PP_SPLIT( \
        0 \
      , HBOOST_PARAMETER_FOR_EACH_pred_aux0( \
            HBOOST_PP_TUPLE_ELEM(5,3,state) \
          , HBOOST_PP_TUPLE_ELEM(5,0,state) \
        ) \
    )
# endif

# define HBOOST_PARAMETER_FOR_EACH_op(r, state) \
    ( \
        HBOOST_PP_TUPLE_EAT(HBOOST_PP_TUPLE_ELEM(5,3,state)) \
          HBOOST_PP_TUPLE_ELEM(5,0,state) \
      , HBOOST_PP_TUPLE_ELEM(5,1,state) \
      , HBOOST_PP_TUPLE_ELEM(5,2,state) \
      , HBOOST_PP_TUPLE_ELEM(5,3,state) \
      , HBOOST_PP_INC(HBOOST_PP_TUPLE_ELEM(5,4,state)) \
    )

# define HBOOST_PARAMETER_FOR_EACH_macro(r, state) \
    HBOOST_PP_TUPLE_ELEM(5,2,state)( \
        r \
      , HBOOST_PP_TUPLE_ELEM(5,4,state) \
      , HBOOST_PARAMETER_FOR_EACH_head( \
            HBOOST_PP_TUPLE_ELEM(5,3,state) \
          , HBOOST_PP_TUPLE_ELEM(5,0,state) \
        ) \
      , HBOOST_PP_TUPLE_ELEM(5,1,state) \
    )

# define HBOOST_PARAMETER_FOR_EACH_build_end_sentinel(z,n,text) \
    HBOOST_PP_COMMA_IF(n) HBOOST_PARAMETER_FOR_EACH_END_SENTINEL
# define HBOOST_PARAMETER_FOR_EACH_build_end_sentinel_tuple(arity) \
    ( \
        HBOOST_PP_REPEAT(arity, HBOOST_PARAMETER_FOR_EACH_build_end_sentinel, _) \
    )

# define HBOOST_PARAMETER_FOR_EACH_R(r, arity, list, data, macro) \
    HBOOST_PP_CAT(HBOOST_PP_FOR_, r)( \
        (list HBOOST_PARAMETER_FOR_EACH_build_end_sentinel_tuple(arity), data, macro, arity, 0) \
      , HBOOST_PARAMETER_FOR_EACH_pred \
      , HBOOST_PARAMETER_FOR_EACH_op \
      , HBOOST_PARAMETER_FOR_EACH_macro \
    )

# define HBOOST_PARAMETER_FOR_EACH(arity, list, data, macro) \
    HBOOST_PARAMETER_FOR_EACH_R(HBOOST_PP_DEDUCE_R(), arity, list, data, macro)

#endif // HBOOST_PARAMETER_FOR_EACH_051217_HPP

