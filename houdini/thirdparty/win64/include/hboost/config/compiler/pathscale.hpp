//  (C) Copyright Bryce Lelbach 2011

//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

// PathScale EKOPath C++ Compiler

#ifndef HBOOST_COMPILER
#  define HBOOST_COMPILER "PathScale EKOPath C++ Compiler version " __PATHSCALE__
#endif

#if __PATHCC__ >= 4
#  define HBOOST_MSVC6_MEMBER_TEMPLATES
#  define HBOOST_HAS_UNISTD_H
#  define HBOOST_HAS_STDINT_H
#  define HBOOST_HAS_SIGACTION
#  define HBOOST_HAS_SCHED_YIELD
#  define HBOOST_HAS_THREADS
#  define HBOOST_HAS_PTHREADS
#  define HBOOST_HAS_PTHREAD_YIELD
#  define HBOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE
#  define HBOOST_HAS_PARTIAL_STD_ALLOCATOR
#  define HBOOST_HAS_NRVO
#  define HBOOST_HAS_NL_TYPES_H
#  define HBOOST_HAS_NANOSLEEP
#  define HBOOST_HAS_LONG_LONG
#  define HBOOST_HAS_LOG1P
#  define HBOOST_HAS_GETTIMEOFDAY
#  define HBOOST_HAS_EXPM1
#  define HBOOST_HAS_DIRENT_H
#  define HBOOST_HAS_CLOCK_GETTIME
#  define HBOOST_NO_CXX11_VARIADIC_TEMPLATES
#  define HBOOST_NO_CXX11_UNICODE_LITERALS
#  define HBOOST_NO_CXX11_TEMPLATE_ALIASES
#  define HBOOST_NO_CXX11_STATIC_ASSERT
#  define HBOOST_NO_SFINAE_EXPR
#  define HBOOST_NO_CXX11_SCOPED_ENUMS
#  define HBOOST_NO_CXX11_RVALUE_REFERENCES
#  define HBOOST_NO_CXX11_RANGE_BASED_FOR
#  define HBOOST_NO_CXX11_RAW_LITERALS
#  define HBOOST_NO_CXX11_NULLPTR
#  define HBOOST_NO_CXX11_NUMERIC_LIMITS
#  define HBOOST_NO_CXX11_NOEXCEPT
#  define HBOOST_NO_CXX11_LAMBDAS
#  define HBOOST_NO_CXX11_LOCAL_CLASS_TEMPLATE_PARAMETERS
#  define HBOOST_NO_MS_INT64_NUMERIC_LIMITS
#  define HBOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
#  define HBOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS
#  define HBOOST_NO_CXX11_DELETED_FUNCTIONS
#  define HBOOST_NO_CXX11_DEFAULTED_FUNCTIONS
#  define HBOOST_NO_CXX11_DECLTYPE
#  define HBOOST_NO_CXX11_DECLTYPE_N3276
#  define HBOOST_NO_CXX11_CONSTEXPR
#  define HBOOST_NO_COMPLETE_VALUE_INITIALIZATION
#  define HBOOST_NO_CXX11_CHAR32_T
#  define HBOOST_NO_CXX11_CHAR16_T
#  define HBOOST_NO_CXX11_AUTO_MULTIDECLARATIONS
#  define HBOOST_NO_CXX11_AUTO_DECLARATIONS
#  define HBOOST_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX
#  define HBOOST_NO_CXX11_HDR_UNORDERED_SET
#  define HBOOST_NO_CXX11_HDR_UNORDERED_MAP
#  define HBOOST_NO_CXX11_HDR_TYPEINDEX
#  define HBOOST_NO_CXX11_HDR_TUPLE
#  define HBOOST_NO_CXX11_HDR_THREAD
#  define HBOOST_NO_CXX11_HDR_SYSTEM_ERROR
#  define HBOOST_NO_CXX11_HDR_REGEX
#  define HBOOST_NO_CXX11_HDR_RATIO
#  define HBOOST_NO_CXX11_HDR_RANDOM
#  define HBOOST_NO_CXX11_HDR_MUTEX
#  define HBOOST_NO_CXX11_HDR_INITIALIZER_LIST
#  define HBOOST_NO_CXX11_HDR_FUTURE
#  define HBOOST_NO_CXX11_HDR_FORWARD_LIST
#  define HBOOST_NO_CXX11_HDR_CONDITION_VARIABLE
#  define HBOOST_NO_CXX11_HDR_CODECVT
#  define HBOOST_NO_CXX11_HDR_CHRONO
#  define HBOOST_NO_CXX11_USER_DEFINED_LITERALS
#  define HBOOST_NO_CXX11_ALIGNAS
#  define HBOOST_NO_CXX11_TRAILING_RESULT_TYPES
#  define HBOOST_NO_CXX11_INLINE_NAMESPACES
#  define HBOOST_NO_CXX11_REF_QUALIFIERS
#  define HBOOST_NO_CXX11_FINAL

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
#endif
