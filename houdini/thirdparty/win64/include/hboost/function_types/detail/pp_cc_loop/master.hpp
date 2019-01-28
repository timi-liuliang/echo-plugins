
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

// no include guards, this file is intended for multiple inclusions

#ifdef __WAVE__
// this file has been generated from the master.hpp file in the same directory
#   pragma wave option(preserve: 0)
#endif


#if !HBOOST_PP_IS_ITERATING

#   ifndef HBOOST_FT_DETAIL_CC_LOOP_MASTER_HPP_INCLUDED
#   define HBOOST_FT_DETAIL_CC_LOOP_MASTER_HPP_INCLUDED
#     include <hboost/function_types/config/cc_names.hpp>

#     include <hboost/preprocessor/cat.hpp>
#     include <hboost/preprocessor/seq/size.hpp>
#     include <hboost/preprocessor/seq/elem.hpp>
#     include <hboost/preprocessor/tuple/elem.hpp>
#     include <hboost/preprocessor/iteration/iterate.hpp>
#     include <hboost/preprocessor/facilities/expand.hpp>
#     include <hboost/preprocessor/arithmetic/inc.hpp>
#   endif

#   include <hboost/function_types/detail/encoding/def.hpp>
#   include <hboost/function_types/detail/encoding/aliases_def.hpp>

#   define  HBOOST_PP_FILENAME_1 \
        <hboost/function_types/detail/pp_cc_loop/master.hpp>
#   define  HBOOST_PP_ITERATION_LIMITS \
        (0,HBOOST_PP_SEQ_SIZE(HBOOST_FT_CC_NAMES_SEQ)-1)
#   include HBOOST_PP_ITERATE()
#   if !defined(HBOOST_FT_config_valid) && HBOOST_FT_CC_PREPROCESSING
#     define HBOOST_FT_cc_id 1
#     define HBOOST_FT_cc_name implicit_cc
#     define HBOOST_FT_cc HBOOST_PP_EMPTY
#     define HBOOST_FT_cond callable_builtin
#     include HBOOST_FT_cc_file
#     undef HBOOST_FT_cond
#     undef HBOOST_FT_cc_name
#     undef HBOOST_FT_cc
#     undef HBOOST_FT_cc_id
#   elif !defined(HBOOST_FT_config_valid) // and generating preprocessed file
HBOOST_PP_EXPAND(#) ifndef HBOOST_FT_config_valid
HBOOST_PP_EXPAND(#)   define HBOOST_FT_cc_id 1
HBOOST_PP_EXPAND(#)   define HBOOST_FT_cc_name implicit_cc
HBOOST_PP_EXPAND(#)   define HBOOST_FT_cc HBOOST_PP_EMPTY
HBOOST_PP_EXPAND(#)   define HBOOST_FT_cond callable_builtin
#define _()
HBOOST_PP_EXPAND(#)   include HBOOST_FT_cc_file
#undef _
HBOOST_PP_EXPAND(#)   undef HBOOST_FT_cond
HBOOST_PP_EXPAND(#)   undef HBOOST_FT_cc_name
HBOOST_PP_EXPAND(#)   undef HBOOST_FT_cc
HBOOST_PP_EXPAND(#)   undef HBOOST_FT_cc_id
HBOOST_PP_EXPAND(#) else
HBOOST_PP_EXPAND(#)   undef HBOOST_FT_config_valid
HBOOST_PP_EXPAND(#) endif

#   else
#     undef HBOOST_FT_config_valid
#   endif

#   include <hboost/function_types/detail/encoding/aliases_undef.hpp>
#   include <hboost/function_types/detail/encoding/undef.hpp>

#elif HBOOST_FT_CC_PREPROCESSING

#   define HBOOST_FT_cc_id  HBOOST_PP_INC(HBOOST_PP_FRAME_ITERATION(1))
#   define HBOOST_FT_cc_inf \
        HBOOST_PP_SEQ_ELEM(HBOOST_PP_FRAME_ITERATION(1),HBOOST_FT_CC_NAMES_SEQ)

#   define HBOOST_FT_cc_pp_name HBOOST_PP_TUPLE_ELEM(3,0,HBOOST_FT_cc_inf)
#   define HBOOST_FT_cc_name    HBOOST_PP_TUPLE_ELEM(3,1,HBOOST_FT_cc_inf)
#   define HBOOST_FT_cc         HBOOST_PP_TUPLE_ELEM(3,2,HBOOST_FT_cc_inf)

#   define HBOOST_FT_cond HBOOST_PP_CAT(HBOOST_FT_CC_,HBOOST_FT_cc_pp_name)

#   if HBOOST_FT_cond
#     define HBOOST_FT_config_valid 1
#     include HBOOST_FT_cc_file
#   endif

#   undef HBOOST_FT_cond

#   undef HBOOST_FT_cc_pp_name
#   undef HBOOST_FT_cc_name
#   undef HBOOST_FT_cc

#   undef HBOOST_FT_cc_id
#   undef HBOOST_FT_cc_inf

#else // if generating preprocessed file
HBOOST_PP_EXPAND(#) define HBOOST_FT_cc_id HBOOST_PP_INC(HBOOST_PP_ITERATION())

#   define HBOOST_FT_cc_inf \
        HBOOST_PP_SEQ_ELEM(HBOOST_PP_ITERATION(),HBOOST_FT_CC_NAMES_SEQ)

#   define HBOOST_FT_cc_pp_name HBOOST_PP_TUPLE_ELEM(3,0,HBOOST_FT_cc_inf)

#   define HBOOST_FT_CC_DEF(name,index) \
        name HBOOST_PP_TUPLE_ELEM(3,index,HBOOST_FT_cc_inf)
HBOOST_PP_EXPAND(#) define HBOOST_FT_CC_DEF(HBOOST_FT_cc_name,1)
HBOOST_PP_EXPAND(#) define HBOOST_FT_CC_DEF(HBOOST_FT_cc,2)
#   undef  HBOOST_FT_CC_DEF

#   define HBOOST_FT_cc_cond_v HBOOST_PP_CAT(HBOOST_FT_CC_,HBOOST_FT_cc_pp_name)
HBOOST_PP_EXPAND(#) define HBOOST_FT_cond HBOOST_FT_cc_cond_v
#   undef  HBOOST_FT_cc_cond_v

#   undef HBOOST_FT_cc_pp_name
#   undef HBOOST_FT_cc_inf

HBOOST_PP_EXPAND(#) if HBOOST_FT_cond
HBOOST_PP_EXPAND(#)   define HBOOST_FT_config_valid 1
#define _()
HBOOST_PP_EXPAND(#)   include HBOOST_FT_cc_file
#undef _
HBOOST_PP_EXPAND(#) endif

HBOOST_PP_EXPAND(#) undef HBOOST_FT_cond

HBOOST_PP_EXPAND(#) undef HBOOST_FT_cc_name
HBOOST_PP_EXPAND(#) undef HBOOST_FT_cc

HBOOST_PP_EXPAND(#) undef HBOOST_FT_cc_id

#endif

