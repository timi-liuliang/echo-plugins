/*
Copyright Rene Rivera 2013
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef HBOOST_PREDEF_DETAIL_ENDIAN_COMPAT_H
#define HBOOST_PREDEF_DETAIL_ENDIAN_COMPAT_H

#include <hboost/predef/other/endian.h>

#if HBOOST_ENDIAN_BIG_BYTE
#   define HBOOST_BIG_ENDIAN
#   define HBOOST_BYTE_ORDER 4321
#endif
#if HBOOST_ENDIAN_LITTLE_BYTE
#   define HBOOST_LITTLE_ENDIAN
#   define HBOOST_BYTE_ORDER 1234
#endif
#if HBOOST_ENDIAN_LITTLE_WORD
#   define HBOOST_PDP_ENDIAN
#   define HBOOST_BYTE_ORDER 2134
#endif

#endif
