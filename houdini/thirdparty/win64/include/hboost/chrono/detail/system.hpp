//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#ifndef HBOOST_CHRONO_DETAIL_SYSTEM_HPP
#define HBOOST_CHRONO_DETAIL_SYSTEM_HPP

#if !defined HBOOST_CHRONO_DONT_PROVIDE_HYBRID_ERROR_HANDLING

#include <hboost/version.hpp>
#include <hboost/system/error_code.hpp>

#if ((HBOOST_VERSION / 100000) < 2) && ((HBOOST_VERSION / 100 % 1000) < 44)
#define HBOOST_CHRONO_SYSTEM_CATEGORY hboost::system::system_category
#else
#define HBOOST_CHRONO_SYSTEM_CATEGORY hboost::system::system_category()
#endif

#ifdef HBOOST_SYSTEM_NO_DEPRECATED
#define HBOOST_CHRONO_THROWS hboost::throws()
#define HBOOST_CHRONO_IS_THROWS(EC) (&EC==&hboost::throws())
#else
#define HBOOST_CHRONO_THROWS hboost::system::throws
#define HBOOST_CHRONO_IS_THROWS(EC) (&EC==&hboost::system::throws)
#endif

#endif
#endif
