#ifndef HBOOST_CORE_NO_EXCEPTIONS_SUPPORT_HPP
#define HBOOST_CORE_NO_EXCEPTIONS_SUPPORT_HPP

#if defined(_MSC_VER)
#  pragma once
#endif

//----------------------------------------------------------------------
// (C) Copyright 2004 Pavel Vozenilek.
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
// This file contains helper macros used when exception support may be
// disabled (as indicated by macro HBOOST_NO_EXCEPTIONS).
//
// Before picking up these macros you may consider using RAII techniques
// to deal with exceptions - their syntax can be always the same with 
// or without exception support enabled.
//----------------------------------------------------------------------

#include <hboost/config.hpp>
#include <hboost/detail/workaround.hpp>

#if !(defined HBOOST_NO_EXCEPTIONS)
#    define HBOOST_TRY { try
#    define HBOOST_CATCH(x) catch(x)
#    define HBOOST_RETHROW throw;
#    define HBOOST_CATCH_END }
#else
#    if HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))
#        define HBOOST_TRY { if ("")
#        define HBOOST_CATCH(x) else if (!"")
#    else
#        define HBOOST_TRY { if (true)
#        define HBOOST_CATCH(x) else if (false)
#    endif
#    define HBOOST_RETHROW
#    define HBOOST_CATCH_END }
#endif


#endif 
