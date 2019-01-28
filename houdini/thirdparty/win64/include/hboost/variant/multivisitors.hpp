//  Boost.Varaint
//  Multivisitors defined here 
//
//  See http://www.boost.org for most recent version, including documentation.
//
//  Copyright Antony Polukhin, 2013-2014.
//
//  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef HBOOST_VARIANT_MULTIVISITORS_HPP
#define HBOOST_VARIANT_MULTIVISITORS_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/config.hpp>
#include "hboost/variant/variant_fwd.hpp" // for HBOOST_VARIANT_DO_NOT_USE_VARIADIC_TEMPLATES

#if !defined(HBOOST_VARIANT_DO_NOT_USE_VARIADIC_TEMPLATES) && !defined(HBOOST_NO_CXX11_HDR_TUPLE)
#   include <hboost/variant/detail/multivisitors_cpp11_based.hpp>
#   if !defined(HBOOST_NO_CXX14_DECLTYPE_AUTO) && !defined(HBOOST_NO_CXX11_DECLTYPE_N3276)
#       include <hboost/variant/detail/multivisitors_cpp14_based.hpp>
#   endif
#else
#   include <hboost/variant/detail/multivisitors_preprocessor_based.hpp>
#endif

#endif // HBOOST_VARIANT_MULTIVISITORS_HPP

