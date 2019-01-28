//  (C) Copyright John Maddock 2001.
//  (C) Copyright Jens Maurer 2001 - 2003.
//  (C) Copyright Peter Dimov 2002.
//  (C) Copyright Aleksey Gurtovoy 2002 - 2003.
//  (C) Copyright David Abrahams 2002.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

//  Sun C++ compiler setup:

#    if __SUNPRO_CC <= 0x500
#      define HBOOST_NO_MEMBER_TEMPLATES
#      define HBOOST_NO_FUNCTION_TEMPLATE_ORDERING
#    endif

#    if (__SUNPRO_CC <= 0x520)
       //
       // Sunpro 5.2 and earler:
       //
       // although sunpro 5.2 supports the syntax for
       // inline initialization it often gets the value
       // wrong, especially where the value is computed
       // from other constants (J Maddock 6th May 2001)
#      define HBOOST_NO_INCLASS_MEMBER_INITIALIZATION

       // Although sunpro 5.2 supports the syntax for
       // partial specialization, it often seems to
       // bind to the wrong specialization.  Better
       // to disable it until suppport becomes more stable
       // (J Maddock 6th May 2001).
#      define HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#    endif

#    if (__SUNPRO_CC <= 0x530)
       // Requesting debug info (-g) with Boost.Python results
       // in an internal compiler error for "static const"
       // initialized in-class.
       //    >> Assertion:   (../links/dbg_cstabs.cc, line 611)
       //         while processing ../test.cpp at line 0.
       // (Jens Maurer according to Gottfried Ganssauge 04 Mar 2002)
#      define HBOOST_NO_INCLASS_MEMBER_INITIALIZATION

       // SunPro 5.3 has better support for partial specialization,
       // but breaks when compiling std::less<shared_ptr<T> >
       // (Jens Maurer 4 Nov 2001).

       // std::less specialization fixed as reported by George
       // Heintzelman; partial specialization re-enabled
       // (Peter Dimov 17 Jan 2002)

//#      define HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

       // integral constant expressions with 64 bit numbers fail
#      define HBOOST_NO_INTEGRAL_INT64_T
#    endif

#    if (__SUNPRO_CC < 0x570)
#      define HBOOST_NO_TEMPLATE_TEMPLATES
       // see http://lists.boost.org/MailArchives/hboost/msg47184.php
       // and http://lists.boost.org/MailArchives/hboost/msg47220.php
#      define HBOOST_NO_INCLASS_MEMBER_INITIALIZATION
#      define HBOOST_NO_SFINAE
#      define HBOOST_NO_ARRAY_TYPE_SPECIALIZATIONS
#    endif
#    if (__SUNPRO_CC <= 0x580)
#      define HBOOST_NO_IS_ABSTRACT
#    endif

#    if (__SUNPRO_CC <= 0x5100)
       // Sun 5.10 may not correctly value-initialize objects of
       // some user defined types, as was reported in April 2010
       // (CR 6947016), and confirmed by Steve Clamage.
       // (Niels Dekker, LKEB, May 2010).
#      define HBOOST_NO_COMPLETE_VALUE_INITIALIZATION
#    endif

//
// Dynamic shared object (DSO) and dynamic-link library (DLL) support
//
#if __SUNPRO_CC > 0x500
#  define HBOOST_SYMBOL_EXPORT __global
#  define HBOOST_SYMBOL_IMPORT __global
#  define HBOOST_SYMBOL_VISIBLE __global
#endif

#if (__SUNPRO_CC < 0x5130)
// C++03 features in 12.4:
#define HBOOST_NO_TWO_PHASE_NAME_LOOKUP
#define HBOOST_NO_SFINAE_EXPR
#define HBOOST_NO_ADL_BARRIER
#define HBOOST_NO_CXX11_VARIADIC_MACROS
#endif

#if (__SUNPRO_CC < 0x5130) || (__cplusplus < 201100)
// C++11 only featuires in 12.4:
#define HBOOST_NO_CXX11_AUTO_DECLARATIONS
#define HBOOST_NO_CXX11_AUTO_MULTIDECLARATIONS
#define HBOOST_NO_CXX11_CHAR16_T
#define HBOOST_NO_CXX11_CHAR32_T
#define HBOOST_NO_CXX11_CONSTEXPR
#define HBOOST_NO_CXX11_DECLTYPE
#define HBOOST_NO_CXX11_DEFAULTED_FUNCTIONS
#define HBOOST_NO_CXX11_DELETED_FUNCTIONS
#define HBOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS
#define HBOOST_NO_CXX11_EXTERN_TEMPLATE
#define HBOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
#define HBOOST_NO_CXX11_HDR_INITIALIZER_LIST
#define HBOOST_NO_CXX11_LAMBDAS
#define HBOOST_NO_CXX11_LOCAL_CLASS_TEMPLATE_PARAMETERS
#define HBOOST_NO_CXX11_NOEXCEPT
#define HBOOST_NO_CXX11_NULLPTR
#define HBOOST_NO_CXX11_RANGE_BASED_FOR
#define HBOOST_NO_CXX11_RAW_LITERALS
#define HBOOST_NO_CXX11_RVALUE_REFERENCES
#define HBOOST_NO_CXX11_SCOPED_ENUMS
#define HBOOST_NO_CXX11_STATIC_ASSERT
#define HBOOST_NO_CXX11_TEMPLATE_ALIASES
#define HBOOST_NO_CXX11_UNICODE_LITERALS
#define HBOOST_NO_CXX11_ALIGNAS
#define HBOOST_NO_CXX11_TRAILING_RESULT_TYPES
#define HBOOST_NO_CXX11_INLINE_NAMESPACES
#define HBOOST_NO_CXX11_FINAL
#endif

#if (__SUNPRO_CC < 0x5140) || (__cplusplus < 201103)
#define HBOOST_NO_CXX11_VARIADIC_TEMPLATES
#define HBOOST_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX
#define HBOOST_NO_CXX11_FIXED_LENGTH_VARIADIC_TEMPLATE_EXPANSION_PACKS
#define HBOOST_NO_CXX11_DECLTYPE_N3276
#define HBOOST_NO_CXX11_USER_DEFINED_LITERALS
#define HBOOST_NO_CXX11_REF_QUALIFIERS
#endif

#define HBOOST_NO_COMPLETE_VALUE_INITIALIZATION
//
// C++0x features
//
#  define HBOOST_HAS_LONG_LONG


// C++ 14:
#if !defined(__cpp_aggregate_nsdmi) || (__cpp_aggregate_nsdmi < 201304)
#  define HBOOST_NO_CXX14_AGGREGATE_NSDMI
#endif
#if !defined(__cpp_binary_literals) || (__cpp_binary_literals < 201304)
#  define HBOOST_NO_CXX14_BINARY_LITERALS
#endif
#if !defined(__cpp_constexpr) || (__cpp_constexpr < 201304)
#  define HBOOST_NO_CXX14_CONSTEXPR
#endif
#if !defined(__cpp_decltype_auto) || (__cpp_decltype_auto < 201304)
#  define HBOOST_NO_CXX14_DECLTYPE_AUTO
#endif
#if (__cplusplus < 201304) // There's no SD6 check for this....
#  define HBOOST_NO_CXX14_DIGIT_SEPARATORS
#endif
#if !defined(__cpp_generic_lambdas) || (__cpp_generic_lambdas < 201304)
#  define HBOOST_NO_CXX14_GENERIC_LAMBDAS
#endif
#if !defined(__cpp_init_captures) || (__cpp_init_captures < 201304)
#  define HBOOST_NO_CXX14_INITIALIZED_LAMBDA_CAPTURES
#endif
#if !defined(__cpp_return_type_deduction) || (__cpp_return_type_deduction < 201304)
#  define HBOOST_NO_CXX14_RETURN_TYPE_DEDUCTION
#endif
#if !defined(__cpp_variable_templates) || (__cpp_variable_templates < 201304)
#  define HBOOST_NO_CXX14_VARIABLE_TEMPLATES
#endif
//
// Version
//

#define HBOOST_COMPILER "Sun compiler version " HBOOST_STRINGIZE(__SUNPRO_CC)

//
// versions check:
// we don't support sunpro prior to version 4:
#if __SUNPRO_CC < 0x400
#error "Compiler not supported or configured - please reconfigure"
#endif
//
// last known and checked version is 0x590:
#if (__SUNPRO_CC > 0x590)
#  if defined(HBOOST_ASSERT_CONFIG)
#     error "Unknown compiler version - please run the configure tests and report the results"
#  endif
#endif
