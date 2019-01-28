
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

// no include guards, this file is intended for multiple inclusion

#ifndef HBOOST_FT_PREPROCESSING_MODE
// input:  HBOOST_FT_mfp        0 or 1 <=> member function pointer?
// input:  HBOOST_FT_type_name  HBOOST_FT_type --> "R (* ..._type_name)()" (pass2)
#endif
// input:  HBOOST_FT_syntax     type macro to use
// input:  HBOOST_FT_cc         empty or cc specifier 
// input:  HBOOST_FT_ell        empty or "..."
// input:  HBOOST_FT_cv         empty or cv qualifiers
// input:  HBOOST_FT_flags      single decimal integer encoding the flags
// output: HBOOST_FT_n          number of component types (arity+1)
// output: HBOOST_FT_arity      current arity
// output: HBOOST_FT_type       macro that expands to the type
// output: HBOOST_FT_tplargs(p) template arguments with given prefix
// output: HBOOST_FT_params(p)  parameters with given prefix

#ifdef __WAVE__
#   pragma wave option(preserve: 0)
#endif

#ifndef HBOOST_FT_ARITY_LOOP_IS_ITERATING

#   define HBOOST_FT_AL_PREPROCESSED \
        HBOOST_FT_AL_FILE(HBOOST_FT_al_path,HBOOST_FT_FROM_ARITY,HBOOST_FT_mfp)

#   define HBOOST_FT_AL_FILE(base_path,max_arity,mfp) \
        HBOOST_FT_AL_FILE_I(base_path,max_arity,mfp)
#   define HBOOST_FT_AL_FILE_I(base_path,max_arity,mfp) \
        <base_path/arity ## max_arity ## _ ## mfp.hpp>

#   if !defined(HBOOST_FT_PREPROCESSING_MODE)

#     if HBOOST_FT_MAX_ARITY < 10
#       define HBOOST_FT_FROM_ARITY 0 
#     elif HBOOST_FT_MAX_ARITY < 20
#       define HBOOST_FT_FROM_ARITY 10
#     elif HBOOST_FT_MAX_ARITY < 30
#       define HBOOST_FT_FROM_ARITY 20
#     elif HBOOST_FT_MAX_ARITY < 40
#       define HBOOST_FT_FROM_ARITY 30
#     endif

#     if HBOOST_FT_FROM_ARITY
#       include HBOOST_FT_AL_PREPROCESSED
#     endif

#   elif !defined(HBOOST_FT_FROM_ARITY) // single pass preprocessing
#     define HBOOST_FT_FROM_ARITY 0

#   elif HBOOST_FT_FROM_ARITY > 0       // arity20 includes arity10
HBOOST_PP_EXPAND(#) include HBOOST_FT_AL_PREPROCESSED
#   endif

#   undef HBOOST_FT_AL_PREPROCESSED

#   undef HBOOST_FT_AL_FILE
#   undef HBOOST_FT_AL_FILE_I

#   if HBOOST_FT_MAX_ARITY > HBOOST_FT_FROM_ARITY

#     ifndef HBOOST_FT_DETAIL_ARITY_LOOP_HPP_INCLUDED
#     define HBOOST_FT_DETAIL_ARITY_LOOP_HPP_INCLUDED
#         include <hboost/preprocessor/cat.hpp>
#         include <hboost/preprocessor/tuple/eat.hpp>
#         include <hboost/preprocessor/control/if.hpp>
#         include <hboost/preprocessor/arithmetic/inc.hpp>
#         include <hboost/preprocessor/facilities/empty.hpp>
#         include <hboost/preprocessor/facilities/expand.hpp>
#         include <hboost/preprocessor/iteration/iterate.hpp>
#         include <hboost/preprocessor/repetition/enum_params.hpp>
#         include <hboost/preprocessor/repetition/enum_shifted_params.hpp>
#         include <hboost/preprocessor/repetition/enum_trailing_params.hpp>
#     endif

#     define HBOOST_FT_AL_INCLUDE_FILE <HBOOST_FT_al_path/master.hpp>

#     define HBOOST_FT_ARITY_LOOP_PREFIX 1
#     include HBOOST_FT_AL_INCLUDE_FILE
#     undef  HBOOST_FT_ARITY_LOOP_PREFIX

#     if !HBOOST_PP_IS_ITERATING
#       define HBOOST_PP_FILENAME_1 HBOOST_FT_AL_INCLUDE_FILE
#     elif HBOOST_PP_ITERATION_DEPTH() == 1
#       define HBOOST_PP_FILENAME_2 HBOOST_FT_AL_INCLUDE_FILE
#     else
#       error "loops nested too deeply"
#     endif

#     define HBOOST_FT_arity HBOOST_PP_ITERATION()
#     define HBOOST_FT_n     HBOOST_PP_INC(HBOOST_FT_arity)

#     define HBOOST_FT_type \
          HBOOST_FT_syntax(HBOOST_FT_cc,HBOOST_FT_type_name HBOOST_PP_EMPTY)\
               (HBOOST_FT_params(HBOOST_PP_EMPTY) HBOOST_FT_ell) HBOOST_FT_cv

#     define HBOOST_FT_tplargs(prefx) \
          prefx() R HBOOST_PP_ENUM_TRAILING_PARAMS(HBOOST_FT_arity,prefx() T)

#     if !HBOOST_FT_mfp

#       define HBOOST_FT_params(prefx) \
            HBOOST_PP_IF(HBOOST_FT_arity,HBOOST_PP_ENUM_PARAMS, \
                HBOOST_FT_nullary_param HBOOST_PP_TUPLE_EAT(2))( \
                    HBOOST_FT_arity,prefx() T) 
#     else

#       define HBOOST_FT_params(prefx) \
            HBOOST_PP_ENUM_SHIFTED_PARAMS(HBOOST_FT_arity,prefx() T)

#     endif

#     if !HBOOST_FT_FROM_ARITY 
#       define HBOOST_PP_ITERATION_LIMITS (HBOOST_FT_mfp, HBOOST_FT_MAX_ARITY)
#     else
#       define HBOOST_PP_ITERATION_LIMITS \
            (HBOOST_FT_FROM_ARITY+1, HBOOST_FT_MAX_ARITY)
#     endif

#     define HBOOST_FT_ARITY_LOOP_IS_ITERATING 1
#     include HBOOST_PP_ITERATE()
#     undef  HBOOST_FT_ARITY_LOOP_IS_ITERATING

#     undef HBOOST_FT_arity
#     undef HBOOST_FT_params
#     undef HBOOST_FT_tplargs
#     undef HBOOST_FT_type

#     define HBOOST_FT_ARITY_LOOP_SUFFIX 1
#     include HBOOST_FT_AL_INCLUDE_FILE
#     undef  HBOOST_FT_ARITY_LOOP_SUFFIX

#     undef HBOOST_FT_AL_INCLUDE_FILE
#   endif

#   undef HBOOST_FT_FROM_ARITY

#else
#   error "attempt to nest arity loops"
#endif

