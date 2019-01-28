
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

// no include guards, this file is intended for multiple inclusions

#ifndef HBOOST_FT_DETAIL_PP_LOOP_HPP_INCLUDED
#define HBOOST_FT_DETAIL_PP_LOOP_HPP_INCLUDED
#   include <hboost/preprocessor/facilities/expand.hpp>
#   include <hboost/preprocessor/facilities/empty.hpp>
#   include <hboost/preprocessor/punctuation/paren.hpp>
#endif

#include <hboost/function_types/detail/encoding/def.hpp>
#include <hboost/function_types/detail/encoding/aliases_def.hpp>

#if defined(HBOOST_FT_PREPROCESSING_MODE)
#   define HBOOST_FT_loop <hboost/function_types/detail/pp_cc_loop/master.hpp>
#else
#   define HBOOST_FT_loop \
        <hboost/function_types/detail/pp_cc_loop/preprocessed.hpp>
#endif

#if defined(HBOOST_FT_al_path)

#   define HBOOST_FT_cc_file \
        <hboost/function_types/detail/pp_variate_loop/preprocessed.hpp>
#   define HBOOST_FT_variate_file \
        <hboost/function_types/detail/pp_arity_loop.hpp>

#   ifndef HBOOST_FT_type_function
#   define HBOOST_FT_type_function(cc,name) HBOOST_FT_SYNTAX( \
      R HBOOST_PP_EMPTY,HBOOST_PP_EMPTY,cc,HBOOST_PP_EMPTY,name,HBOOST_PP_EMPTY)
#   endif
#   ifndef HBOOST_FT_type_function_pointer
#   define HBOOST_FT_type_function_pointer(cc,name) HBOOST_FT_SYNTAX( \
      R HBOOST_PP_EMPTY,HBOOST_PP_LPAREN,cc,* HBOOST_PP_EMPTY,name,HBOOST_PP_RPAREN)
#   endif
#   ifndef HBOOST_FT_type_function_reference
#   define HBOOST_FT_type_function_reference(cc,name) HBOOST_FT_SYNTAX( \
      R HBOOST_PP_EMPTY,HBOOST_PP_LPAREN,cc,& HBOOST_PP_EMPTY,name,HBOOST_PP_RPAREN)
#   endif
#   ifndef HBOOST_FT_type_member_function_pointer
#   define HBOOST_FT_type_member_function_pointer(cc,name) HBOOST_FT_SYNTAX( \
      R HBOOST_PP_EMPTY,HBOOST_PP_LPAREN,cc,T0::* HBOOST_PP_EMPTY,name,HBOOST_PP_RPAREN)
#   endif

#   include HBOOST_FT_loop

#   undef HBOOST_FT_type_function
#   undef HBOOST_FT_type_function_pointer
#   undef HBOOST_FT_type_function_reference
#   undef HBOOST_FT_type_member_function_pointer

#   undef HBOOST_FT_variations
#   undef HBOOST_FT_variate_file
#   undef HBOOST_FT_cc_file
#   undef HBOOST_FT_al_path

#elif defined(HBOOST_FT_cc_file)

#   include HBOOST_FT_loop
#   undef HBOOST_FT_cc_file

#else

#   error "argument missing"

#endif

#undef HBOOST_FT_loop

#include <hboost/function_types/detail/encoding/aliases_undef.hpp>
#include <hboost/function_types/detail/encoding/undef.hpp>


