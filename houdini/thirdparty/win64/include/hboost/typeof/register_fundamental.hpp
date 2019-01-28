// Copyright (C) 2004 Arkadiy Vertleyb
// Copyright (C) 2004 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_TYPEOF_REGISTER_FUNDAMENTAL_HPP_INCLUDED
#define HBOOST_TYPEOF_REGISTER_FUNDAMENTAL_HPP_INCLUDED

#include <hboost/typeof/typeof.hpp>

#include HBOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

HBOOST_TYPEOF_REGISTER_TYPE(unsigned char)
HBOOST_TYPEOF_REGISTER_TYPE(unsigned short)
HBOOST_TYPEOF_REGISTER_TYPE(unsigned int)
HBOOST_TYPEOF_REGISTER_TYPE(unsigned long)

HBOOST_TYPEOF_REGISTER_TYPE(signed char)
HBOOST_TYPEOF_REGISTER_TYPE(signed short)
HBOOST_TYPEOF_REGISTER_TYPE(signed int)
HBOOST_TYPEOF_REGISTER_TYPE(signed long)

HBOOST_TYPEOF_REGISTER_TYPE(bool)
HBOOST_TYPEOF_REGISTER_TYPE(char)

HBOOST_TYPEOF_REGISTER_TYPE(float)
HBOOST_TYPEOF_REGISTER_TYPE(double)
HBOOST_TYPEOF_REGISTER_TYPE(long double)

#ifndef HBOOST_NO_INTRINSIC_WCHAR_T
// If the following line fails to compile and you're using the Intel
// compiler, see http://lists.boost.org/MailArchives/boost-users/msg06567.php,
// and define HBOOST_NO_INTRINSIC_WCHAR_T on the command line.
HBOOST_TYPEOF_REGISTER_TYPE(wchar_t)
#endif

#if (defined(HBOOST_MSVC) && (HBOOST_MSVC == 1200)) \
    || (defined(HBOOST_INTEL_CXX_VERSION) && defined(_MSC_VER) && (HBOOST_INTEL_CXX_VERSION <= 600)) \
    || (defined(__BORLANDC__) && (__BORLANDC__ == 0x600) && (_MSC_VER == 1200))
HBOOST_TYPEOF_REGISTER_TYPE(unsigned __int8)
HBOOST_TYPEOF_REGISTER_TYPE(__int8)
HBOOST_TYPEOF_REGISTER_TYPE(unsigned __int16)
HBOOST_TYPEOF_REGISTER_TYPE(__int16)
HBOOST_TYPEOF_REGISTER_TYPE(unsigned __int32)
HBOOST_TYPEOF_REGISTER_TYPE(__int32)
#ifdef __BORLANDC__
HBOOST_TYPEOF_REGISTER_TYPE(unsigned __int64)
HBOOST_TYPEOF_REGISTER_TYPE(__int64)
#endif
#endif

# if defined(HBOOST_HAS_LONG_LONG)
HBOOST_TYPEOF_REGISTER_TYPE(::hboost::ulong_long_type)
HBOOST_TYPEOF_REGISTER_TYPE(::hboost::long_long_type)
#elif defined(HBOOST_HAS_MS_INT64)
HBOOST_TYPEOF_REGISTER_TYPE(unsigned __int64)
HBOOST_TYPEOF_REGISTER_TYPE(__int64)
#endif

HBOOST_TYPEOF_REGISTER_TYPE(void)

#endif//HBOOST_TYPEOF_REGISTER_FUNDAMENTAL_HPP_INCLUDED
