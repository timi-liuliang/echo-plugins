
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef HBOOST_FT_CONFIG_HPP_INCLUDED
#define HBOOST_FT_CONFIG_HPP_INCLUDED

#include <hboost/function_types/config/compiler.hpp>
#include <hboost/function_types/config/cc_names.hpp>

// maximum allowed arity
#ifndef HBOOST_FT_MAX_ARITY
#define HBOOST_FT_MAX_ARITY 20
#endif

// the most common calling conventions for x86 architecture can be enabled at
// once in the compiler config
#ifdef HBOOST_FT_COMMON_X86_CCs
#   ifndef HBOOST_FT_CC_CDECL
#   define HBOOST_FT_CC_CDECL HBOOST_FT_COMMON_X86_CCs
#   endif
#   ifndef HBOOST_FT_CC_STDCALL
#   define HBOOST_FT_CC_STDCALL non_variadic|HBOOST_FT_COMMON_X86_CCs
#   endif
#   ifndef HBOOST_FT_CC_FASTCALL
#   define HBOOST_FT_CC_FASTCALL non_variadic|HBOOST_FT_COMMON_X86_CCs
#   endif
#endif

// where to place the cc specifier (the common way)
#ifndef HBOOST_FT_SYNTAX
#   define HBOOST_FT_SYNTAX(result,lparen,cc_spec,type_mod,name,rparen) \
                        result() lparen() cc_spec() type_mod() name() rparen()
#endif

// param for nullary functions
// set to "void" for compilers that require nullary functions to read 
// "R (void)" in template partial specialization
#ifndef HBOOST_FT_NULLARY_PARAM
#define HBOOST_FT_NULLARY_PARAM 
#endif

// there is a pending defect report on cv qualified function types, so support
// for these types is disabled, unless for compilers where it's known to work
#ifndef HBOOST_FT_NO_CV_FUNC_SUPPORT
#define HBOOST_FT_NO_CV_FUNC_SUPPORT 1
#endif

// full preprocessing implies preprocessing of the ccs
#if defined(HBOOST_FT_PREPROCESSING_MODE) && !defined(HBOOST_FT_CC_PREPROCESSING)
#   define HBOOST_FT_CC_PREPROCESSING 1
#endif

#endif

