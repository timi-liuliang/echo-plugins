
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifdef __WAVE__
// this file has been generated from the master.hpp file in the same directory
#   pragma wave option(preserve: 0)
#endif

#if !defined(HBOOST_FT_PREPROCESSING_MODE)
#   error "this file is only for two-pass preprocessing"
#endif

#if !defined(HBOOST_PP_VALUE)
#   include <hboost/preprocessor/slot/slot.hpp>
#   include <hboost/preprocessor/facilities/empty.hpp>
#   include <hboost/preprocessor/facilities/expand.hpp>
#   include <hboost/function_types/detail/encoding/def.hpp>

HBOOST_PP_EXPAND(#) define HBOOST_FT_mfp 0
HBOOST_PP_EXPAND(#) define HBOOST_FT_syntax HBOOST_FT_type_function

#   define  HBOOST_PP_VALUE \
      HBOOST_FT_function|HBOOST_FT_non_variadic
#   include __FILE__
#   define  HBOOST_PP_VALUE \
      HBOOST_FT_function|HBOOST_FT_variadic
#   include __FILE__

HBOOST_PP_EXPAND(#) if !HBOOST_FT_NO_CV_FUNC_SUPPORT
#   define  HBOOST_PP_VALUE \
      HBOOST_FT_function|HBOOST_FT_non_variadic|HBOOST_FT_const
#   include __FILE__
#   define  HBOOST_PP_VALUE \
      HBOOST_FT_function|HBOOST_FT_variadic|HBOOST_FT_const
#   include __FILE__
#   define  HBOOST_PP_VALUE \
      HBOOST_FT_function|HBOOST_FT_non_variadic|HBOOST_FT_volatile
#   include __FILE__
#   define  HBOOST_PP_VALUE \
      HBOOST_FT_function|HBOOST_FT_variadic|HBOOST_FT_volatile
#   include __FILE__
#   define  HBOOST_PP_VALUE \
      HBOOST_FT_function|HBOOST_FT_non_variadic|HBOOST_FT_const|HBOOST_FT_volatile
#   include __FILE__
#   define  HBOOST_PP_VALUE \
      HBOOST_FT_function|HBOOST_FT_variadic|HBOOST_FT_const|HBOOST_FT_volatile
#   include __FILE__
HBOOST_PP_EXPAND(#) endif


HBOOST_PP_EXPAND(#) undef  HBOOST_FT_syntax
HBOOST_PP_EXPAND(#) define HBOOST_FT_syntax HBOOST_FT_type_function_pointer

#   define  HBOOST_PP_VALUE \
      HBOOST_FT_pointer|HBOOST_FT_non_variadic
#   include __FILE__
#   define  HBOOST_PP_VALUE \
      HBOOST_FT_pointer|HBOOST_FT_variadic
#   include __FILE__

HBOOST_PP_EXPAND(#) undef  HBOOST_FT_syntax
HBOOST_PP_EXPAND(#) define HBOOST_FT_syntax HBOOST_FT_type_function_reference

#   define HBOOST_PP_VALUE \
      HBOOST_FT_reference|HBOOST_FT_non_variadic
#   include __FILE__
#   define HBOOST_PP_VALUE \
      HBOOST_FT_reference|HBOOST_FT_variadic
#   include __FILE__

HBOOST_PP_EXPAND(#) undef  HBOOST_FT_syntax
HBOOST_PP_EXPAND(#) undef  HBOOST_FT_mfp

HBOOST_PP_EXPAND(#) define HBOOST_FT_mfp 1
HBOOST_PP_EXPAND(#) define HBOOST_FT_syntax HBOOST_FT_type_member_function_pointer

#   define  HBOOST_PP_VALUE \
      HBOOST_FT_member_function_pointer|HBOOST_FT_non_variadic
#   include __FILE__
#   define  HBOOST_PP_VALUE \
      HBOOST_FT_member_function_pointer|HBOOST_FT_variadic
#   include __FILE__
#   define  HBOOST_PP_VALUE \
      HBOOST_FT_member_function_pointer|HBOOST_FT_non_variadic|HBOOST_FT_const
#   include __FILE__
#   define  HBOOST_PP_VALUE \
      HBOOST_FT_member_function_pointer|HBOOST_FT_variadic|HBOOST_FT_const
#   include __FILE__
#   define  HBOOST_PP_VALUE \
      HBOOST_FT_member_function_pointer|HBOOST_FT_non_variadic|HBOOST_FT_volatile
#   include __FILE__
#   define  HBOOST_PP_VALUE \
      HBOOST_FT_member_function_pointer|HBOOST_FT_variadic|HBOOST_FT_volatile
#   include __FILE__
#   define  HBOOST_PP_VALUE \
      HBOOST_FT_member_function_pointer|HBOOST_FT_non_variadic|HBOOST_FT_const|HBOOST_FT_volatile
#   include __FILE__
#   define  HBOOST_PP_VALUE \
      HBOOST_FT_member_function_pointer|HBOOST_FT_variadic|HBOOST_FT_const|HBOOST_FT_volatile
#   include __FILE__

HBOOST_PP_EXPAND(#) undef  HBOOST_FT_syntax
HBOOST_PP_EXPAND(#) undef  HBOOST_FT_mfp

#   include <hboost/function_types/detail/encoding/undef.hpp>
#else 

#   include HBOOST_PP_ASSIGN_SLOT(1)

#   define  HBOOST_PP_VALUE HBOOST_PP_SLOT(1) & HBOOST_FT_kind_mask
#   include HBOOST_PP_ASSIGN_SLOT(2)

HBOOST_PP_EXPAND(#) if !!(HBOOST_PP_SLOT(2) & (HBOOST_FT_variations))
HBOOST_PP_EXPAND(#) if (HBOOST_PP_SLOT(1) & (HBOOST_FT_cond)) == (HBOOST_FT_cond)

#   if ( HBOOST_PP_SLOT(1) & (HBOOST_FT_variadic) )
HBOOST_PP_EXPAND(#)   define HBOOST_FT_ell ...
HBOOST_PP_EXPAND(#)   define HBOOST_FT_nullary_param
#   else
HBOOST_PP_EXPAND(#)   define HBOOST_FT_ell
HBOOST_PP_EXPAND(#)   define HBOOST_FT_nullary_param HBOOST_FT_NULLARY_PARAM
#   endif

#   if !( HBOOST_PP_SLOT(1) & (HBOOST_FT_volatile) )
#     if !( HBOOST_PP_SLOT(1) & (HBOOST_FT_const) )
HBOOST_PP_EXPAND(#)   define HBOOST_FT_cv 
#     else
HBOOST_PP_EXPAND(#)   define HBOOST_FT_cv const
#     endif
#   else
#     if !( HBOOST_PP_SLOT(1) & (HBOOST_FT_const) )
HBOOST_PP_EXPAND(#)   define HBOOST_FT_cv volatile
#     else
HBOOST_PP_EXPAND(#)   define HBOOST_FT_cv const volatile
#     endif
#   endif
HBOOST_PP_EXPAND(#)   define HBOOST_FT_flags HBOOST_PP_SLOT(1)
HBOOST_PP_EXPAND(#)   include HBOOST_FT_variate_file

HBOOST_PP_EXPAND(#)   undef HBOOST_FT_cv
HBOOST_PP_EXPAND(#)   undef HBOOST_FT_ell
HBOOST_PP_EXPAND(#)   undef HBOOST_FT_nullary_param
HBOOST_PP_EXPAND(#)   undef HBOOST_FT_flags
HBOOST_PP_EXPAND(#) endif
HBOOST_PP_EXPAND(#) endif
#endif

